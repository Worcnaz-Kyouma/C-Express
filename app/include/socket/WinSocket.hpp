#ifndef CE_WINSOCKET_H
#define CE_WINSOCKET_H

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#include "socket/Socket.hpp"

#pragma comment(lib, "Ws2_32.lib")

class WinSocket : public Socket {
private:
    SOCKET socket;

    static const int BUFFER_SIZE = 30720000; // 30 MB
    char buffer[BUFFER_SIZE];
public:
    WinSocket();
    ~WinSocket() override;

    virtual void bindSocket(unsigned int port) = 0;
    virtual void listenSocket(unsigned int numOfWaitingConnections) = 0;
    virtual Socket* acceptSocket() = 0;

    virtual const char* const readSocket() = 0;
    virtual void writeSocket(const char* message, const size_t messageSize) const = 0;
};

#endif