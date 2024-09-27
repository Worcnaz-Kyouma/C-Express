#ifdef _WIN32
#include "socket/WinSocket.hpp"

#include <iostream>
#include <cstring>

#include <stdexcept>

WinSocket::WinSocket():
    socketFD(INVALID_SOCKET) {}

WinSocket::WinSocket(SOCKET socketFD):
    socketFD(socketFD) {}

WinSocket::~WinSocket() {
    if(this->socketFD != INVALID_SOCKET) 
        closesocket(this->socketFD);
}

void WinSocket::bindSocket(unsigned int port) {
    if(port < 2000 || port > 65535) {
        throw std::runtime_error("Invalid port number.\nMust be between 2000 and 65535.");
    }

    if(this->socketFD != INVALID_SOCKET) {
        throw std::runtime_error("Port already binded.");
    }

    struct addrinfo* sockAddrInfo = NULL;
    struct addrinfo hints;

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    bool isGetAddrInfoSucess = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &sockAddrInfo) == 0;
    if(!isGetAddrInfoSucess) {
        throw std::runtime_error("Error while trying to get address info");
    }

    this->socketFD = socket(
        sockAddrInfo->ai_family,
        sockAddrInfo->ai_socktype,
        sockAddrInfo->ai_protocol
    );
    if(this->socketFD == INVALID_SOCKET) {
        freeaddrinfo(sockAddrInfo);
        throw std::runtime_error("Error while trying to create socket");
    }

    int opt = 1;
    if (setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) == SOCKET_ERROR) {
        throw std::runtime_error("Error setting socket options: " + std::to_string(WSAGetLastError()));
    }

    bool isBindSuccess = bind(this->socketFD, sockAddrInfo->ai_addr, (int)sockAddrInfo->ai_addrlen) != SOCKET_ERROR;
    if(!isBindSuccess) {
        freeaddrinfo(sockAddrInfo);
        throw std::runtime_error("Error while trying to bind socket to the given port");
    }

    freeaddrinfo(sockAddrInfo);
}

// numOfWaitingConnections not used on Windows lib
void WinSocket::listenSocket(unsigned int numOfWaitingConnections) {
    if(this->socketFD == INVALID_SOCKET) {
        throw std::runtime_error("Socket not created and binded");
    }

    bool isListenSuccess = listen(this->socketFD, SOMAXCONN) != SOCKET_ERROR;
    if(!isListenSuccess) {
        throw std::runtime_error("Error while trying to configure listen of socket");
    }
}

Socket* WinSocket::acceptSocket() {
    SOCKET clientSocketFD = INVALID_SOCKET;

    clientSocketFD = accept(this->socketFD, NULL, NULL);
    if(clientSocketFD == INVALID_SOCKET) {
        throw std::runtime_error("Error while trying to accept new client connections");
    }

    WinSocket* clientSocket = new WinSocket(clientSocketFD);
    return clientSocket;
}

const char* const WinSocket::readSocket() {
    std::memset(this->buffer, 0, this->BUFFER_SIZE);

    const int numOfBytesRead = recv(this->socketFD, this->buffer, this->BUFFER_SIZE, 0);

    if(numOfBytesRead < 0) {
        throw std::runtime_error("Error in data read from socket.");
    }

    return this->buffer;
}


void WinSocket::writeSocket(const char* message, const size_t messageSize) const {
    const int numOfBytesWritten = send(this->socketFD, message, messageSize, 0);

    if(numOfBytesWritten == SOCKET_ERROR) {
        throw std::runtime_error("Error in data write on socket.");
    }
}
#endif