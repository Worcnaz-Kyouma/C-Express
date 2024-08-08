#ifndef CE_SOCKETUNIX_H
#define CE_SOCKETUNIX_H

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "Socket.hpp"

class SocketUnix : public Socket {
private:
    int socketFD;

    const static int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    struct sockaddr_in socketAddress;
public:
    SocketUnix();
    explicit SocketUnix(int socketFD);


    void bindSocket(unsigned int port) override;
    void listenSocket(unsigned int numOfWaitingConnections) override;
    Socket* acceptSocket() override;

    char* readSocket() override;
    void writeSocket(char* message) override;

};

#endif