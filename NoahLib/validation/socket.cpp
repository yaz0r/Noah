#include "noahLib.h"
#include "socket.h"
#pragma comment (lib, "Ws2_32.lib")
#include <WinSock2.h>

bool openSocket(hSocket& socket, const std::string& address, u16 port) {
#ifdef WIN32
    static bool initialized = false;
    if (!initialized) {
        WSAData wsa = { };
        int result = WSAStartup(MAKEWORD(2, 2), &wsa);
        assert(result == 0);
        initialized = true;
    }
#endif
    socket = ::socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in client = { };

    client.sin_family = AF_INET;
    client.sin_port = htons(port);
    client.sin_addr.s_addr = ::inet_addr(address.c_str());

    int connectResult = ::connect(socket, reinterpret_cast<sockaddr*>(&client), sizeof(client));
    return connectResult == 0;
}

void writeBytes(const hSocket& socket, const char* bytes, int size) {
    ::send(socket, bytes, size, 0);
}

void writeString(const hSocket& socket, const std::string& packet) {
    writeBytes(socket, (char*)packet.data(), packet.size());
}

int readBytes(const hSocket& socket, char* bytes, int size) {
    return ::recv(socket, bytes, size, 0);
}

char readCharacter(const hSocket& socket) {
    char data;
    readBytes(socket, &data, 1);
    return data;
}