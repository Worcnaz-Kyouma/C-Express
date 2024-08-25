#include "Server.hpp"
#include "Utils.hpp"
#include <stdexcept>
#include <iostream>

Server::Server() {
    this->serverSocket = Socket::getSocket();
}

void Server::get(std::string rawEndpoint, void (*handler)(Request, Response)) { 
    this->httpController->addResource(GET, rawEndpoint, handler); 
}
void Server::post(std::string rawEndpoint, void (*handler)(Request, Response)) { 
    this->httpController->addResource(POST, rawEndpoint, handler); 
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
    auto clientProcess = std::make_unique<std::thread>(&Server::processRequest, this, newClientRequest);

    this->serverThreads.push_back(std::move(clientProcess));
}

void Server::processRequest(Socket* clientSocket) {
    const char* rawRequest = clientSocket->readSocket();
    
    auto [ requestedProcess, request, response ] = this->httpController.handleRequest(rawRequest);

    requestedProcess(request, response);
}