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
    auto clientProcess = std::make_unique<std::thread>(&Server::processRequest, this, newClientRequest);

    this->serverThreads.push_back(std::move(clientProcess));
}

void Server::processRequest(Socket* clientSocket) {
    const char* rawRequest = clientSocket->readSocket();
    
    Request request = Server::httpParser.parseRequest(rawRequest);
    Response response(clientSocket);

    std::map<Endpoint, Process>::iterator requestedResource = this->findResource(request);

    request = Server::httpParser.prepareRequest(request, requestedResource->first); // URL Params => data

    Process requestedProcess = requestedResource->second;

    requestedProcess(request, response);
}

std::map<Endpoint, Process>::iterator Server::findResource(Request request) {
    std::vector<std::string> endpoint = Utils::split(request.url, '/');
    endpoint.insert(endpoint.begin(), request.method);

    auto matchedEndpoint = this->getResourceEndpoint(endpoint);
    if(matchedEndpoint.size() == 0) {
        //response error... thing about it, return a error process
    } else {
        auto targetEndpoint = matchedEndpoint[0];
        auto targetProcess = this->resources.find(targetEndpoint);

        return targetProcess;
    }
}

std::vector<Endpoint> Server::getResourceEndpoint(Endpoint targetEndpoint) {
    //Get endpoint with equal size to the target
    std::vector<Endpoint> endpoints;
    for(const auto& resource : this->resources) {
        Endpoint endpoint = resource.first;
        if(endpoint.size() == targetEndpoint.size())
            endpoints.push_back(resource.first);
    }
    
    //Get the endpoint and its possibles generics cause URL Params
    for(int i = 0; endpoints.size() > 1; i++) {
        for(auto ptr = endpoints.begin(); ptr != endpoints.end(); ptr++){
            if((*ptr)[i][0] != ':' && (*ptr)[i] != targetEndpoint[i]) {
                endpoints.erase(ptr);
            }
        }
    }

    //No generic endpoint disturbing the flow
    if(endpoints.size() < 2) {
        return endpoints;
    }

    //Remove the endpoint with the least count of static parts of the URL
    const std::string genericFragmentIdentifier = "/:";
    std::pair<int, Endpoint> leastCountOfGenericFragment = {std::numeric_limits<int>::max(), endpoints[0]};
    for(auto endpointPtr = endpoints.begin(); endpointPtr != endpoints.end(); endpointPtr++) {
        int countOfGenericFragment = 0;
        for(const auto& endpointFragPtr : *endpointPtr) {
            if(endpointFragPtr.find(genericFragmentIdentifier) != std::string::npos)
                countOfGenericFragment++;
        }

        if(countOfGenericFragment < leastCountOfGenericFragment.first) {
            leastCountOfGenericFragment = {countOfGenericFragment, *endpointPtr};
        }
    }

    endpoints = {leastCountOfGenericFragment.second};
    
    return endpoints;
}

void Server::promptResources() {
    for(auto& resouce : this->resources) {
        for(auto& endpointFrag : resouce.first) {
            std::cout << '/' << endpointFrag;
        }
        std::cout << std::endl;
    }
}