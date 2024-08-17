#include "Utils.hpp"
#include "HTTPParser.hpp"
#include <iostream>
#include <sstream>

Endpoint HTTPParser::parseEndpoint(const std::string& rawEndpoint, const std::string& method) const {
    Endpoint endpoint = Utils::split(rawEndpoint, '/');

    //Endpoint input validations
    if(endpoint.size() == 0) {
        throw std::runtime_error("Error, empty string can't be a endpoint!");
    } else if(!endpoint[0].empty()) {
        throw std::runtime_error("Error, endpoint need to begin with a single slash '/'");
    }
    
    //Remove empty char generated in left-side first slash
    endpoint.erase(endpoint.begin());

    endpoint.insert(endpoint.begin(), method);

    return endpoint;
}

Request HTTPParser::parseRequest(const std::string& rawRequest) const {
    std::vector<std::string> requestRows = Utils::split(rawRequest, '\n');
    
    //request line parse(first line of request)
    std::vector<std::string> requestLine = Utils::split(requestRows[0], ' ');
    
    Request parsedRequest(
        requestLine[0], // Method
        requestLine[1], // Endpoint
        requestLine[2]  // Protocol
    );

    return parsedRequest;
}

Request HTTPParser::prepareRequest(Request request, Endpoint endpoint) const {
    return request;
}