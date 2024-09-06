#include "HTTPController.hpp"
#include "Utils.hpp"
#include <algorithm>
#include <optional>
#include <stdexcept>

HTTPController::HTTPController(Server* server, AvailableHTTPProtocols protocol):
server(server),
httpParser(HTTPParser::getHTTPParser(protocol, this)),
httpCore(new HTTPCore()) {}

Endpoint HTTPController::parseRawEndpoint(const std::string& rawEndpoint) {
    if(rawEndpoint.size() == 0) {
        throw new std::runtime_error("Empty string can't be a endpoint");
    }
    if(rawEndpoint[0] != '/') {
        throw new std::runtime_error("String of the endpoint must begin with a forward slash");
    }

    Endpoint endpoint = Utils::split(rawEndpoint, '/');
    if(hasDuplicateGenericFrag(endpoint)) {
        throw new std::runtime_error("Can't have duplicated generic frag identifier");
    }

    for(auto epFragment = endpoint.begin(); epFragment!=endpoint.end()-1; epFragment++)
        if(epFragment->find('?') != std::string::npos) 
            throw new std::runtime_error("Invalid question mark in the url");
    

    return endpoint;
}

ResourceManager HTTPController::getResourceManager(Endpoint endpoint, const std::string& method) const {
    std::optional<Resource> resourceOpt = this->httpCore->getResource(endpoint);
    if(!resourceOpt.has_value()) return nullptr;

    Resource resource = *resourceOpt;

    try {
        ResourceOperation resourceOperation = resource.second.at(method);

        ResourceManager resourceManager = resourceOperation.second;
    } catch(const std::out_of_range& e) {
        return nullptr;
    }
}

void HTTPController::addResource(const std::string& rawMethod, const std::string& rawEndpoint, ResourceManager resourceManager) {
    Method method = this->httpParser->parseMethod(rawMethod);

    Endpoint endpoint = this->parseRawEndpoint(rawEndpoint);

    ResourceManager foundRSManager = this->getResourceManager(endpoint, method);
    if(foundRSManager != nullptr) throw std::runtime_error("Method already defined to that endpoint.");

    this->httpCore->addResourceOperation(
        endpoint, 
        std::make_pair(Method(method), resourceManager)
    );
}

Process HTTPController::getProcess(const std::string& rawRequest) {
    std::optional<Request> requestOpt = this->httpParser->generateRequest(rawRequest);
    if(!requestOpt.has_value()) { // Error 400, broken HTTP request
        auto [ resourceManager, request, response ] = this->httpParser->getGenericsRM(400);

        return std::make_tuple(resourceManager, request, response);
    } 

    Request request = *requestOpt;
    ResourceManager rsManager = this->getResourceManager(
        request.sysEndpoint, 
        this->httpParser->parseMethod(request.method, true)
    );
    if(rsManager == nullptr) { // Error 404, no resource found
        auto [ resourceManager, request, response ] = this->httpParser->getGenericsRM(404);

        return std::make_tuple(resourceManager, request, response);
    }

    Response response = this->httpParser->generateResponse(requestOpt);

    return std::make_tuple(rsManager, request, response);
}

//Inner methods functions implementation 
bool hasDuplicateGenericFrag(Endpoint endpoint) {
    Endpoint endpointJustGenerics;

    std::copy_if(endpoint.begin(), endpoint.end(), std::back_inserter(endpointJustGenerics), 
        [](std::string fragment) {
            return fragment[0] == ':';
        }
    );

    if(endpointJustGenerics.empty()) return false;

    return Utils::hasDuplicate(endpointJustGenerics);
}