#pragma once

#ifdef WIN32
typedef uintptr_t hSocket;
#else
#error No socket implementation
#endif

bool openSocket(hSocket& socket, const std::string& address, u16 port);
void writeString(const hSocket& socket, const std::string& packet);
char readCharacter(const hSocket& socket);