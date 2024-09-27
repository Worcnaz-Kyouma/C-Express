#ifdef _WIN32
#ifndef CE_WINSOCKET_H
#define CE_WINSOCKET_H

#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "socket/Socket.hpp"

#pragma comment(lib, "Ws2_32.lib")

class WinSocket : public Socket {
private:
    SOCKET socketFD;

    static const int BUFFER_SIZE = 30720000; // 30 MB
    char buffer[BUFFER_SIZE];
public:
    WinSocket();
    explicit WinSocket(SOCKET socketFD);
    ~WinSocket() override;

    void bindSocket(unsigned int port) override;
    void listenSocket(unsigned int numOfWaitingConnections) override;
    Socket* acceptSocket() override;

    const char* const readSocket() override;
    void writeSocket(const char* message, const size_t messageSize) const override;
};

#endif
#endif