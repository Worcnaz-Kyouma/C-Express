#include "Utils.hpp"
#include "HTTPParser.hpp"
#include <iostream>
#include <sstream>

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