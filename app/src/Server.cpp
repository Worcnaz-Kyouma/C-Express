#include "Server.hpp"
#include "Utils.hpp"
#include <stdexcept>
#include <iostream>

Server::Server():
    serverSocket(Socket::getSocket()),
    httpController(new HTTPController(this, HTTP1x0)) {}

Server::Server(AvailableHTTPProtocols protocol):
    serverSocket(Socket::getSocket()),
    httpController(new HTTPController(this, protocol)) {}

void Server::get(std::string rawEndpoint, ResourceManager resourceManager) { 
    this->httpController->addResource("get", rawEndpoint, resourceManager); 
}
void Server::post(std::string rawEndpoint, ResourceManager resourceManager) { 
    this->httpController->addResource("post", rawEndpoint, resourceManager); 
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
    
    auto [ resourceManager, request, response ] = this->httpController->getProcess(rawRequest, clientSocket);

    resourceManager(request, response);
}

void Server::sendResponse(const std::string& response, const Socket* clientSocket) {
    clientSocket->writeSocket(response.c_str(), response.size());
}