#include "http/HTTPCore.hpp"
#include "stdexcept"
#include "algorithm"
#include "iostream"

void HTTPCore::addResourceOperation(Endpoint endpoint, ResourceOperation newResourceOperation) {
    auto [ method, rsOperation ] = newResourceOperation;
    
    auto resource = this->resources.find(endpoint);
    if(resource == this->resources.end()) {
        ResourceOperations newRSOperations;
        newRSOperations.insert({ method, rsOperation });

        this->resources.insert({ endpoint, newRSOperations });
        this->insertOrderedEndpoints.push_back(endpoint);
        return;
    }
    
    ResourceOperations rsOperations = resource->second;
    if(rsOperations.find(method) != rsOperations.end()) {
        throw std::runtime_error("Method already defined to that endpoint");
    }

    rsOperations.insert({
        method,
        rsOperation
    });
}

std::optional<ResourceManager> HTTPCore::getResourceManager(Endpoint sysEndpoint, Method sysMethod) {
    try {
        ResourceOperations rsOperations = resources.at(sysEndpoint);

        ResourceManager rsManager = rsOperations.at(sysMethod);

        return rsManager;
    } catch(const std::out_of_range& e) {
        return std::nullopt;
    }
}

std::vector<Endpoint> HTTPCore::getEndpoints(const unsigned int& numberOfFragments) {
    if(numberOfFragments == 0) return this->insertOrderedEndpoints;

    std::vector<Endpoint> filteredEndpoints;
    
    std::copy_if(
        this->insertOrderedEndpoints.begin(),
        this->insertOrderedEndpoints.end(),
        std::back_inserter(filteredEndpoints),
        [numberOfFragments] (const Endpoint& endpoint) {
            return endpoint.size() == numberOfFragments;
        }
    );

    return filteredEndpoints;
}