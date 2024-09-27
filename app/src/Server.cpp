#include "Server.hpp"
#include "Utils.hpp"
#include <stdexcept>
#include <iostream>
#include <memory>

#ifdef _WIN32
    #include <winsock2.h>
#endif

Server::Server(): Server::Server(HTTP1x0) {}

Server::Server(AvailableHTTPProtocols protocol):
serverSocket(Socket::getSocket()),
httpController(new HTTPController(this, protocol)) {
    #ifdef _WIN32
        WSADATA wsaData;

        bool isWSAStartupSuccess = WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
        if(!isWSAStartupSuccess) {
            std::runtime_error("Failed to startup Win Socket resources");
        }
    #endif
}

Server::~Server() {
    delete this->serverSocket;
    delete this->httpController;

    #ifdef _WIN32
        WSACleanup();
    #endif
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

    delete request;
    delete response;
}

void Server::sendResponse(const std::string& response, const Socket* clientSocket) {
    std::cout << "Response: "<< response.c_str() << std::endl;
    std::cout << std::endl;

    // should be 179?
    std::cout << "Response size: "<< response.size() << std::endl;
    std::cout << std::endl;

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