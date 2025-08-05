#pragma once

#ifdef WIN32
#pragma comment (lib, "Ws2_32.lib")
#include <WinSock2.h>
typedef SOCKET hSocket;
#else
#error No socket implementation
#endif

bool openSocket(hSocket& socket, const std::string& address, u16 port);
void writeString(const hSocket& socket, const std::string& packet);
char readCharacter(const hSocket& socket);