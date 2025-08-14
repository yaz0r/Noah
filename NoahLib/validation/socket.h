#pragma once

namespace socketNS {

#ifdef WIN32
    typedef uintptr_t hSocket;
#else
    typedef int hSocket;
#endif

    bool openSocket(hSocket& socket, const std::string& address, u16 port);
    void writeString(const hSocket& socket, const std::string& packet);
    char readCharacter(const hSocket& socket);
};
