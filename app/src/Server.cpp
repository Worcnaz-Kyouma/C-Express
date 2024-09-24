#include "Server.hpp"
#include "Utils.hpp"
#include <stdexcept>
#include <iostream>
#include <memory>

Server::Server():
    serverSocket(Socket::getSocket()),
    httpController(new HTTPController(this, HTTP1x0)) {}

Server::Server(AvailableHTTPProtocols protocol):
    serverSocket(Socket::getSocket()),
    httpController(new HTTPController(this, protocol)) {}

Server::~Server() {
    delete this->serverSocket;
    delete this->httpController;
}

void Server::get(std::string rawEndpoint, ResourceManager resourceManager) { 
    this->httpController->addResource("GET", rawEndpoint, resourceManager); 
}
void Server::post(std::string rawEndpoint, ResourceManager resourceManager) { 
    this->httpController->addResource("POST", rawEndpoint, resourceManager); 
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

    // We got a problem here, in the request delete...
    delete request;
    delete response;
}

void Server::sendResponse(const std::string& response, const Socket* clientSocket) {
    clientSocket->writeSocket(response.c_str(), response.size());
}

void Server::promptEndpoints() {
    std::vector<Endpoint> endpoints = this->httpController->getEndpoints();

    int count = 0;
    std::for_each(endpoints.begin(), endpoints.end(), [&count](auto endpoint) {
        count++;
        std::cout << "Endpoint: " << count << std::endl;
        
        std::for_each(endpoint.begin(), endpoint.end(), [](auto frag) {
            std::cout << frag << "/";
        });
        std::cout << std::endl;
    });
};