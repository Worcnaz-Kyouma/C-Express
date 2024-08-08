#ifndef CE_SOCKETUNIX_H
#define CE_SOCKETUNIX_H

#include <stdexcept>
#include <cstring>

#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "Socket.hpp"

class SocketUnix : public Socket {
private:
    int socketFD;

    static const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    struct sockaddr_in socketAddress;
public:
    SocketUnix();
    explicit SocketUnix(int socketFD);
    ~SocketUnix();

    void bindSocket(unsigned int port) override;
    void listenSocket(unsigned int numOfWaitingConnections) override;
    Socket* acceptSocket() override;

    const char* const readSocket() override;
    void writeSocket(char* message, int messageSiz) override;

};

#endif