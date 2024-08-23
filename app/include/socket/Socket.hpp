#ifndef CE_SOCKET_H
#define CE_SOCKET_H

class Socket {
public:
    static Socket* getSocket();

    virtual void bindSocket(unsigned int port) = 0;
    virtual void listenSocket(unsigned int numOfWaitingConnections) = 0;
    virtual Socket* acceptSocket() = 0;

    virtual const char* const readSocket() = 0;
    virtual void writeSocket(char* message, int messageSize) = 0;
};

#endif