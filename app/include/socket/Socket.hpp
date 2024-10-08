#ifndef CE_SOCKET_H
#define CE_SOCKET_H

#include <cstddef>

class Socket {
public:
    virtual ~Socket() {};
    static Socket* getSocket();

    virtual void bindSocket(unsigned int port) = 0;
    virtual void listenSocket(unsigned int numOfWaitingConnections) = 0;
    virtual Socket* acceptSocket() = 0;

    virtual const char* const readSocket() = 0;
    virtual void writeSocket(const char* message, const size_t messageSize) const = 0;
};

#endif