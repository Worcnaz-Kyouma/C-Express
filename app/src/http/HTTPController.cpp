#include "http/HTTPController.hpp"
#include "Utils.hpp"
#include "iostream"
#include <algorithm>
#include <optional>
#include <stdexcept>

// Implementing functions
bool hasDuplicateGenericFrag(Endpoint);

HTTPController::HTTPController(Server* server, AvailableHTTPProtocols protocol):
server(server),
httpParser(HTTPParser::getHTTPParser(protocol, this)),
httpCore(new HTTPCore()) {}

HTTPController::~HTTPController() {
    delete this->httpParser;
    delete this->httpCore;
}

void HTTPController::addResource(const std::string& rawMethod, const std::string& rawEndpoint, ResourceManager resourceManager) {
    Method method = this->httpParser->parseMethod(rawMethod);
    
    Endpoint endpoint = this->parseRawEndpoint(rawEndpoint);
    
    this->httpCore->addResourceOperation(
        endpoint, 
        std::make_pair(Method(method), resourceManager)
    );
}

Process HTTPController::getProcess(const std::string& rawRequest, Socket* clientSocket) {
    Request* request = this->httpParser->generateRequest(rawRequest, clientSocket);
    
    if(request == nullptr || request->isIncomplete) { // Error 400, broken HTTP request
        auto [ resourceManager, request, response ] = this->httpParser->getGenericsRM(400, clientSocket);

        return std::make_tuple(resourceManager, request, response);
    } 

    std::optional<ResourceManager> rsManager = this->httpCore->getResourceManager(
        request->sysEndpoint, 
        this->httpParser->parseMethod(request->method, true)
    );
    if(!rsManager.has_value()) { // Error 404, no resource found
        auto [ resourceManager, request, response ] = this->httpParser->getGenericsRM(404, clientSocket);

        return std::make_tuple(resourceManager, request, response);
    }

    Response* response = this->httpParser->generateResponse(request);

    return std::make_tuple(*rsManager, request, response);
}

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
        throw std::runtime_error("Empty string can't be a endpoint");
    }
    if(rawEndpoint[0] != '/') {
        throw std::runtime_error("String of the endpoint must begin with a forward slash");
    }
    
    Endpoint endpoint = Utils::split(rawEndpoint, '/');
    if(hasDuplicateGenericFrag(endpoint)) {
        throw std::runtime_error("Can't have duplicated generic frag identifier");
    }

    if(endpoint.size() > 1)
        for(auto epFragment = endpoint.begin(); epFragment != endpoint.end() - 1; epFragment++)
            if(epFragment->find('?') != std::string::npos) 
                throw std::runtime_error("Invalid question mark in the url");
        
    
    return endpoint;
}

Protocol HTTPController::parseRawProtocol(const std::string& rawProtocol) {
    if(rawProtocol.size() != 8) {
        throw std::runtime_error("Invalid protocol text size");
    }

    if(rawProtocol.substr(0, 5) != "HTTP/") {
        throw std::runtime_error("Invalid protocol syntax");
    }

    std::string protocolVersion = rawProtocol.substr(5, rawProtocol.size() - 5);
    std::vector<std::string> versions = Utils::split(protocolVersion, '.');
    
    try {
        std::stoi(versions[0]);
        std::stoi(versions[1]);
    } catch(const std::exception& e) {
        throw std::runtime_error("Invalid protocol high or low version");
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

std::pair<StatusCode, StatusDesc> HTTPController::getHTTPStatus(StatusCode newStatus) {
    const std::map<StatusCode, StatusDesc> httpCodes = {
        { 100, "Continue" },
        { 101, "Switching Protocols" },
        { 200, "OK" },
        { 201, "Created" },
        { 202, "Accepted" },
        { 203, "Non-Authoritative Information" },
        { 204, "No Content" },
        { 205, "Reset Content" },
        { 206, "Partial Content" },
        { 300, "Multiple Choices" },
        { 301, "Moved Permanently" },
        { 302, "Found" },
        { 303, "See Other" },
        { 304, "Not Modified" },
        { 305, "Use Proxy" },
        { 307, "Temporary Redirect" },
        { 400, "Bad Request" },
        { 401, "Unauthorized" },
        { 402, "Payment Required" },
        { 403, "Forbidden" },
        { 404, "Not Found" },
        { 405, "Method Not Allowed" },
        { 406, "Not Acceptable" },
        { 407, "Proxy Authentication Required" },
        { 408, "Request Timeout" },
        { 409, "Conflict" },
        { 410, "Gone" },
        { 411, "Length Required" },
        { 412, "Precondition Failed" },
        { 413, "Payload Too Large" },
        { 414, "URI Too Long" },
        { 415, "Unsupported Media Type" },
        { 416, "Range Not Satisfiable" },
        { 417, "Expectation Failed" },
        { 426, "Upgrade Required" },
        { 500, "Internal Server Error" },
        { 501, "Not Implemented" },
        { 502, "Bad Gateway" },
        { 503, "Service Unavailable" },
        { 504, "Gateway Timeout" },
        { 505, "HTTP Version Not Supported" }
    };

    auto newResStatusIt = httpCodes.find(newStatus);
    if(newResStatusIt == httpCodes.end()){
        return { 2036, "... The third war would begin without them." };
    }

    return {
        newResStatusIt->first,
        newResStatusIt->second
    };
}

void HTTPController::sendResponse(Response* response) const {
    const std::string stringfiedResponse = this->httpParser->parseResponse(response);

    this->server->sendResponse(stringfiedResponse, response->requestOrigin->clientSocket);
}

std::vector<Endpoint> HTTPController::getEndpoints() {
    return this->httpCore->getEndpoints();
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

    return Utils::hasDuplicate<std::string>(endpointJustGenerics);
}