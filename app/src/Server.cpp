#include "Server.hpp"
#include "Utils.hpp"
#include <stdexcept>
#include <iostream>

Server::Server() {
    this->serverSocket = Socket::getSocket();
}

void Server::addResource(AvailableMethods method, std::string rawEndpoint, void (*process)(Request, Response)) {
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

    std::vector<std::string> endpoint = Utils::split(rawEndpoint, '/');
    if(endpoint.size() == 0) {
        throw std::runtime_error("Error, invalid endpoint format");
    }
    endpoint.insert(endpoint.begin(), methodStringified);

    std::vector<Endpoint> existingEndpoint = this->getResourceEndpoint(endpoint);
    if(existingEndpoint.size() == 0) {
        this->resources.insert(std::make_pair(endpoint, process));
    } else {
        throw std::runtime_error("Endpoint already defined!");
    }
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

void Server::processRequest(Socket* clientSocket) {
    const char* rawRequest = clientSocket->readSocket();
    
    Request request = Server::httpParser.parseRequest(rawRequest);
    Response response(clientSocket);

    Process requestedProcess = this->findProcess(request);

    requestedProcess(request, response);
}

Process Server::findProcess(Request request) {
    std::vector<std::string> endpoint = Utils::split(request.endpoint, '/');
    endpoint.insert(endpoint.begin(), request.method);

    auto matchedEndpoint = this->getResourceEndpoint(endpoint);
    if(matchedEndpoint.size() == 0) {
        //response error... thing about it, return a error process
    } else {
        auto targetEndpoint = matchedEndpoint[0];
        Process targetProcess = this->resources.find(targetEndpoint)->second;

        return targetProcess;
    }
}

std::vector<Endpoint> Server::getResourceEndpoint(Endpoint targetEndpoint) {
    std::vector<Endpoint> endpoints;
    for(const auto& resource : this->resources) {
        endpoints.push_back(resource.first);
    }
    
    //Url params are fucked
    for(int i = 0; endpoints.size() > 1; i++) {
        for(auto ptr = endpoints.begin(); ptr != endpoints.end(); ptr++){
            if((*ptr)[i][0] != ':' && (*ptr)[i] != targetEndpoint[i]) {
                endpoints.erase(ptr);
            }
        }
    }
    
    return endpoints;
}