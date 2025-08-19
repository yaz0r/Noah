#include "noahLib.h"
#include "gdbConnection.h"

#include <format>
#include <thread>  // For std::this_thread::sleep_for
#include <chrono>  // For std::chrono::seconds, milliseconds, etc.
#include <unordered_set>

#include "sharedmem.h"

using namespace socketNS;

GDBConnection* g_gdbConnection = nullptr;

u8 calculateChecksum(const std::string& data) {
    u64 checksum = 0;
    for (const auto& c : data)
        checksum += c;
    return checksum & 0xFF;
}

std::string createPacket(const std::string& data) {
    return std::format("${}#{:02x}", data, calculateChecksum(data));
}

void sendAck(const hSocket& socket) {
    writeString(socket, "+");
}

std::string sendReceivePackage(const hSocket& socket, const std::string& packet) {
    if (packet.length() > 4) {
        writeString(socket, packet);
    }

    int retries = 20;

    std::string buffer;
    while (true) {
        char c = readCharacter(socket);
        if (c == '+') {
            // ACK response, continue reading
            continue;
        }
        else if (c == '$') {
            // Start of response packet
            buffer += c;

            // Read until the end of the packet
            while (true) {
                c = readCharacter(socket);
                buffer += c;
                if (c == '#')
                    break;
            }

            // Read the checksum
            buffer += readCharacter(socket);
            buffer += readCharacter(socket);
            break;
        }
        else if (c == '-') {
            // NAK response, retry sending the packet
            writeString(socket, packet);
            retries -= 1;
        }
        else if (c == 0x00) {
            // No response, wait a bit and retry receiving data
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            retries -= 1;
        }

        if (retries <= 0) {
            //log::error("No response from GDB server after multiple retries");
            assert(0);
            return {};
        }
    }

    if (!buffer.empty())
        sendAck(socket);

    return buffer;
}

static int fromHexChar(char c) {
    if ((c >= '0') && (c <= '9')) return c - '0';
    if ((c >= 'A') && (c <= 'F')) return c + 10 - 'A';
    if ((c >= 'a') && (c <= 'f')) return c + 10 - 'a';
    return -1;
}

std::string decodeEscapedString(const std::string& input) {
    assert(input[0] == 'O');
    std::string::const_iterator it = input.begin() + 1;
    std::string decodedString;
    decodedString.reserve((input.length() - 1) / 2);
    while (it != input.end()) {
        decodedString.push_back((fromHexChar(it[0]) << 4) | fromHexChar(it[1]));

        it+=2;
    }

    return decodedString;
}

bool GDBConnection::openConnection(const std::string& server, u16 port) {
    if (!openSocket(m_socket, server, port))
        return false;

    sendReceivePackage(m_socket, createPacket("!"));
    sendReceivePackage(m_socket, createPacket("Hg0"));

#ifdef WIN32
    std::string sharedmem = decodeEscapedString(sendMonitorCommand("sharedmem wram"));
    if (sharedmem.length()) {
        m_wramSharedMemory = new SharedMem();
        if (!m_wramSharedMemory->init(sharedmem.c_str(), 0x00800000, false)) {
            delete m_wramSharedMemory;
            m_wramSharedMemory = nullptr;
        }
    }
#endif

    return true;
}

std::optional<std::string> parsePacket(const std::string& packet) {
    if (packet.length() < 4 || packet[0] != '$')
        return std::nullopt;

    size_t hashPos = packet.find('#');
    if (hashPos == std::string::npos || hashPos + 2 >= packet.size())
        return std::nullopt;

    std::string data = packet.substr(1, hashPos - 1);
    std::string checksumStr = packet.substr(hashPos + 1, 2);

   //auto decodedChecksum = decode16(checksumStr);
    //if (decodedChecksum.empty() || decodedChecksum[0] != calculateChecksum(data))
    //    return std::nullopt;

    return data;
}

std::vector<u8> decodeMemoryResponse(const std::string& response) {
    std::string expanded;
    size_t i = 0;

    while (i < response.size()) {
        char c = response[i];

        if (i + 2 < response.size() && response[i + 1] == '*') {
            char repeatChar = response[i + 2];
            int repeatCount = static_cast<unsigned char>(repeatChar) - 29;

            if (repeatCount <= 0)
                return {};

            expanded.append(repeatCount, c);
            i += 3;
        }
        else {
            expanded.push_back(c);
            i++;
        }
    }

    if (expanded.size() % 2 != 0)
        return {};

    std::vector<u8> decoded;
    for (size_t j = 0; j < expanded.size(); j += 2) {
        std::string byteStr = expanded.substr(j, 2);
        try {
            u8 byte = static_cast<u8>(std::stoul(byteStr, nullptr, 16));
            decoded.push_back(byte);
        }
        catch (...) {
            return { };
        }
    }

    return decoded;
}

bool GDBConnection::readMemory(u64 address, void* buffer, size_t size) {
    if (m_wramSharedMemory) {
        u8* pPtrBase = m_wramSharedMemory->getPtr();
        pPtrBase += address & 0x007FFFFF;
        memcpy(buffer, pPtrBase, size);
        return true;
    }
    else {
        std::string packet = createPacket(std::format("m{:X},{:X}", address, size));

        std::string receivedPacket = sendReceivePackage(m_socket, packet);

        auto receivedData = parsePacket(receivedPacket);
        if (!receivedData.has_value())
            return false;

        sendAck(m_socket);

        if (receivedData->size() == 3 && receivedData->starts_with("E"))
            return {};

        auto data = decodeMemoryResponse(*receivedData);
        memcpy(buffer, data.data(), size);
        return true;
    }
}

void GDBConnection::writeMemory(u64 address, const void* buffer, size_t size) {
    if (m_wramSharedMemory) {
        u8* pPtrBase = m_wramSharedMemory->getPtr();
        pPtrBase += address & 0x007FFFFF;
        memcpy(pPtrBase, buffer, size);
    }
    else {
        assert(0);
    }
}

std::unordered_set<u64> m_breakpoints;

void GDBConnection::setBreakpoint(u64 address) {
    if (m_breakpoints.contains(address)) {
        return;
    }
    std::string packet = createPacket(std::format("Z0,{:X},{}", address, 4));
    std::string receivedPacket = sendReceivePackage(m_socket, packet);

    auto receivedData = parsePacket(receivedPacket);
    if (!receivedData.has_value())
        return;

    sendAck(m_socket);

    m_breakpoints.insert(address);
}

void GDBConnection::removeBreakpoint(u64 address) {
    if (!m_breakpoints.contains(address)) {
        return;
    }
    std::string packet = createPacket(std::format("z0,{:X},{}", address, 4));
    std::string receivedPacket = sendReceivePackage(m_socket, packet);

    auto receivedData = parsePacket(receivedPacket);
    if (!receivedData.has_value())
        return;

    sendAck(m_socket);

    m_breakpoints.erase(address);
}

std::string flipStringEndianess(const std::string& string) {
    std::string resultString;
    for (int j = 6; j >= 0; j -= 2) {
        resultString.push_back(string[j]);
        resultString.push_back(string[j + 1]);
    }

    return resultString;
}

void GDBConnection::getRegisters(sRegs& outputRegs) {
    std::string packet = createPacket(std::format("g"));
    std::string receivedPacket = sendReceivePackage(m_socket, packet);

    auto receivedData = parsePacket(receivedPacket);
    if (!receivedData.has_value())
        return;

    const int numRegs = sizeof(outputRegs.rawRegs) / sizeof(outputRegs.rawRegs[0]);
    int position = 0;
    for (int i = 0; i < numRegs; i++) {
        std::string stringValue = flipStringEndianess(receivedData->substr(position, 8));
        outputRegs.rawRegs[i] = std::stoul(stringValue, nullptr, 16);
        position += 8;
        if (receivedData->length() - position < 8) {
            break;
        }
    }

    sendAck(m_socket);
}

std::string GDBConnection::sendMonitorCommand(const std::string& command) {
    std::string commandEncoded;
    for (int i = 0; i < command.length(); i++) {
        commandEncoded += std::format("{:02X}", command[i]);
    }

    std::string packet = createPacket(std::format("qRcmd,{}", commandEncoded));
    std::string receivedPacket = sendReceivePackage(m_socket, packet);

    auto receivedData = parsePacket(receivedPacket);
    if (!receivedData.has_value())
        return "";

    sendAck(m_socket);

    // Wait for OK
    {
        std::string receivedPacket = sendReceivePackage(m_socket, "");
        auto receivedData = parsePacket(receivedPacket);
        if (!receivedData.has_value())
            return "";
        assert(*receivedData == "OK");
    }

    return receivedData.value();
}

void GDBConnection::resetTarget() {
    sendMonitorCommand("reset");
}

u32 GDBConnection::getRegister(int index) {
    std::string packet = createPacket(std::format("p{:02X}", index));

    std::string receivedPacket = sendReceivePackage(m_socket, packet);

    auto receivedData = parsePacket(receivedPacket);
    if (!receivedData.has_value())
        return 0;

    std::string stringValue = flipStringEndianess(*receivedData);
    u32 value = std::stoul(stringValue, nullptr, 16);

    sendAck(m_socket);

    return value;
    /*
    sRegs regs;
    getRegisters(regs);
    return regs.rawRegs[index];
    */
}

void GDBConnection::setRegister(int index, u32 value) {
    value = ((value & 0xff000000) >> 24) | ((value & 0x00ff0000) >> 8) | ((value & 0x0000ff00) << 8) | ((value & 0x000000ff) << 24);
    std::string packet = createPacket(std::format("P{:02X}={:08X}", index, value));

    std::string receivedPacket = sendReceivePackage(m_socket, packet);

    auto receivedData = parsePacket(receivedPacket);
    assert(receivedData.has_value());
    assert((*receivedData) == "OK");

    sendAck(m_socket);
}

u32 GDBConnection::pauseExecution() {
    std::string packet = createPacket("s");
    std::string receivedPacket = sendReceivePackage(m_socket, packet);

    auto receivedData = parsePacket(receivedPacket);
    if (!receivedData.has_value())
        return 0;

    assert(*receivedData == "T05");
    sendAck(m_socket);

    // Return current PC
    return getRegister(37);
}

u32 GDBConnection::executeToNextTrap() {
    std::string packet = createPacket("c");
    std::string receivedPacket = sendReceivePackage(m_socket, packet);

    auto receivedData = parsePacket(receivedPacket);
    if (!receivedData.has_value())
        return 0;

    //sendAck(m_socket);

    while (true) {
        if (receivedData.value_or("") == "T05")
        {
            break;
        }
        std::string receivedPacket = sendReceivePackage(m_socket, createPacket(""));
        auto receivedData = parsePacket(receivedPacket);
        sendAck(m_socket);
        if (receivedData.value_or("") == "T05")
        {
            break;
        }
            
        //else
        //    assert(receivedData.value_or("") == "S00");

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // Return current PC
    return getRegister(37);
}

void GDBConnection::executeUntilAddress(u32 address) {
    setBreakpoint(address);
    u32 PC = executeToNextTrap();
    assert(PC == address);
   // removeBreakpoint(address);
}

u8 GDBConnection::readU8(u64 address) {
    u8 data;
    readMemory(address, &data, 1);
    return data;
}

u16 GDBConnection::readU16(u64 address) {
    u16 data;
    readMemory(address, &data, 2);
    return data;
}

u32 GDBConnection::readU32(u64 address) {
    u32 data;
    readMemory(address, &data, 4);
    return data;
}

void GDBConnection::writeU32(u64 address, u32 value) {
    writeMemory(address, &value, 4);
}

void GDBConnection::writeU16(u64 address, u16 value) {
    writeMemory(address, &value, 2);
}