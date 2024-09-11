#include "HTTPController.hpp"
#include "Utils.hpp"
#include <algorithm>
#include <optional>
#include <stdexcept>

HTTPController::HTTPController(Server* server, AvailableHTTPProtocols protocol):
server(server),
httpParser(HTTPParser::getHTTPParser(protocol, this)),
httpCore(new HTTPCore()) {}

/* gonna be in the HTTPCore
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
}*/

void HTTPController::addResource(const std::string& rawMethod, const std::string& rawEndpoint, ResourceManager resourceManager) {
    Method method = this->httpParser->parseMethod(rawMethod);

    Endpoint endpoint = this->parseRawEndpoint(rawEndpoint);

    this->httpCore->addResourceOperation(
        endpoint, 
        std::make_pair(Method(method), resourceManager)
    );
}

Process HTTPController::getProcess(const std::string& rawRequest) {
    std::optional<Request> requestOpt = this->httpParser->generateRequest(rawRequest);
    if(!requestOpt.has_value() || (*requestOpt).isIncomplete) { // Error 400, broken HTTP request
        auto [ resourceManager, request, response ] = this->httpParser->getGenericsRM(400);

        return std::make_tuple(resourceManager, request, response);
    } 

    Request request = *requestOpt;
    std::optional<ResourceManager> rsManager = this->httpCore->getResourceManager(
        request.sysEndpoint, 
        this->httpParser->parseMethod(request.method, true)
    );
    if(!rsManager.has_value()) { // Error 404, no resource found
        auto [ resourceManager, request, response ] = this->httpParser->getGenericsRM(404);

        return std::make_tuple(resourceManager, request, response);
    }

    Response response = this->httpParser->generateResponse(request);

    return std::make_tuple(*rsManager, request, response);
}

//They need to be ordened in insert order
std::optional<Endpoint> HTTPController::getSysEndpoint(Endpoint source) {
    std::vector<Endpoint> matchedEndpoints = this->httpCore->getEndpoints(source.size());
    if(matchedEndpoints.size() == 0) return std::nullopt;

    for(unsigned int fragIndex = 0; fragIndex < source.size(); fragIndex++) {
        for(auto matchingEndpointPtr = matchedEndpoints.begin(); matchingEndpointPtr != matchedEndpoints.end(); matchingEndpointPtr++) {
            Endpoint matchingEndpoint = *matchingEndpointPtr;
            
            if(matchingEndpoint[fragIndex][0] == ':') continue;

            if(matchingEndpoint[fragIndex] == source[fragIndex]) continue;

            matchedEndpoints.erase(matchingEndpointPtr);
        }

        if(matchedEndpoints.size() == 0) return std::nullopt;
    }

    return matchedEndpoints[0];
}

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

Protocol HTTPController::parseRawProtocol(const std::string& rawProtocol) {
    if(rawProtocol.size() != 8) {
        throw new std::runtime_error("Invalid protocol text size");
    }

    if(rawProtocol.substr(0, 5) != "HTTP/") {
        throw new std::runtime_error("Invalid protocol syntax");
    }

    std::string protocolVersion = rawProtocol.substr(5, rawProtocol.size() - 5);
    std::vector<std::string> versions = Utils::split(protocolVersion, '.');
    
    try {
        std::stoi(versions[0]);
        std::stoi(versions[1]);
    } catch(const std::exception& e) {
        throw new std::runtime_error("Invalid protocol high or low version");
    }
    
    return rawProtocol;
}

bool HTTPController::validateHTTPHeaderNameSyntax(const std::string& headerName) {
    for(auto headerChar = headerName.begin(); headerChar != headerName.end(); headerChar++) {
        const char hc = *headerChar;

        if(hc < 0 || hc > 127) return false; // ASCII range

        if(hc < 32 || hc == 127) return false; // Control characters

        const std::array<char, 16> invalidChars = {
            ':', ';', ',', '/', '@',
            '(', ')', '[', ']', '{', '}',
            '<', '>', 
            '=', '?',' '
        };

        if(std::find(invalidChars.begin(), invalidChars.end(), hc) != invalidChars.end()) 
            return false;

        return true;
    }
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