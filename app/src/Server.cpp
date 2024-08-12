#include "Server.hpp"
#include <stdexcept>
#include <iostream>

Server::Server() {
    this->serverSocket = Socket::getSocket();
}

void Server::addResource(AvailableMethods method, std::string endpoint, void (*process)(Request, Response)) {
    char* methodStringified;
    switch (method) {
        case GET:
            methodStringified = "get";
            break;
        
        case POST:
            methodStringified = "post";
            break;
        
        case PATCH:
            methodStringified = "patch";
            break;

        case DELETE:
            methodStringified = "delete";
            break;
    }

    

    //split endpoint to array with its method on the index 0
    //add the process with the array endpoint to the resources of the server
}

void Server::listen(unsigned int port) {
    try {
        this->serverSocket->bindSocket(port);
        this->serverSocket->listenSocket(5);

        while(true) {
            Socket* clientSocket = this->serverSocket->acceptSocket();

            this->serveRequest(clientSocket);
        }
    } catch (const std::runtime_error& e) {
        std::cout << "Caught error while initializing server listen\nError: " << e.what() << std::endl;
    }
}

void Server::serveRequest(Socket* newClientRequest) {
    std::thread clientProcess(&Server::processRequest, this, newClientRequest);

    this->serverThreads.push_back(clientProcess);
}