#include "socket/SocketUnix.hpp"
#include <iostream>

SocketUnix::SocketUnix() {
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFD < 0) {
        throw std::runtime_error("Error in socket initialization.");
    }

    // Setting Socket to not wait TIME_WAIT state of last used address
    int opt = 1;
    if (setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        throw std::runtime_error("Error setting socket options");
    }


    this->socketFD = socketFD;
    std::memset(this->buffer, 0, this->BUFFER_SIZE);
}

SocketUnix::SocketUnix(int socketFD) :
socketFD(socketFD) {
    std::memset(this->buffer, 0, this->BUFFER_SIZE);
}

SocketUnix::~SocketUnix() {
    close(this->socketFD);
}

void SocketUnix::bindSocket(unsigned int port) {
    if(port < 2000 || port > 65535) {
        throw std::runtime_error("Invalid port number.\nMust be between 2000 and 65535.");
    }

    if(
        this->socketAddress.sin_family == AF_INET &&
        this->socketAddress.sin_addr.s_addr == INADDR_ANY
    ) {
        throw std::runtime_error("Port already binded.");
    }

    this->socketAddress.sin_family = AF_INET;
    this->socketAddress.sin_addr.s_addr = INADDR_ANY;
    this->socketAddress.sin_port = htons(port);

    const int bindResult = bind(
        this->socketFD,
        (struct sockaddr*) &this->socketAddress,
        sizeof(this->socketAddress)
    );
    if(bindResult < 0) {
        throw std::runtime_error("Error trying to bind the socket");
    }
}

void SocketUnix::listenSocket(unsigned int numOfWaitingConnections) {
    if(numOfWaitingConnections == 0 || numOfWaitingConnections > 5) {
        throw std::runtime_error("Invalid number of max waiting connections. The max number is 5.");
    }

    listen(this->socketFD, numOfWaitingConnections);
}

Socket* SocketUnix::acceptSocket() {
    struct sockaddr_in clientSocketAddress;
    socklen_t clientSocketAddressSize = sizeof(clientSocketAddress);

    int clientSocketFD = accept(
        this->socketFD,
        (struct sockaddr*) &clientSocketAddress,
        &clientSocketAddressSize
    );
    if(clientSocketFD < 0) {
        throw std::runtime_error("Error in client socket connection.");
    }

    Socket* clientSocket = new SocketUnix(clientSocketFD);

    return clientSocket;
}

const char* const SocketUnix::readSocket() {
    std::memset(this->buffer, 0, this->BUFFER_SIZE);

    const int numOfBytesRead = read(this->socketFD, this->buffer, this->BUFFER_SIZE);

    if(numOfBytesRead < 0) {
        throw std::runtime_error("Error in data read from socket.");
    }

    return this->buffer;
}

void SocketUnix::writeSocket(const char* message, const size_t messageSize) const {
    const int numOfBytesWritten = write(this->socketFD, message, messageSize);

    if(numOfBytesWritten < 0) {
        throw std::runtime_error("Error in data write on socket.");
    }
}