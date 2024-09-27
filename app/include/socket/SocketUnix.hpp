#ifdef __linux__
#ifndef CE_SOCKETUNIX_H
#define CE_SOCKETUNIX_H

#include <stdexcept>
#include <cstring>

#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "socket/Socket.hpp"

class SocketUnix : public Socket {
private:
    int socketFD;

    static const int BUFFER_SIZE = 30720000; // 30 MB
    char buffer[BUFFER_SIZE];

    struct sockaddr_in socketAddress;
public:
    SocketUnix();
    explicit SocketUnix(int socketFD);
    ~SocketUnix() override;

    void bindSocket(unsigned int port) override;
    void listenSocket(unsigned int numOfWaitingConnections) override;
    Socket* acceptSocket() override;

    const char* const readSocket() override;
    void writeSocket(const char* message, const size_t messageSize) const override;

};

#endif
#endif