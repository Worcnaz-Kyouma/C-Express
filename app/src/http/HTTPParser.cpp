#include "HTTPParser.hpp"
#include "HTTPParser1x0.hpp"
#include "HTTPController.hpp"
#include "Utils.hpp"
#include <stdexcept>
#include <algorithm>

HTTPParser* HTTPParser::getHTTPParser(AvailableHTTPProtocols protocol) {
    switch (protocol){
        case HTTP1x0:
            return new HTTPParser1x0();
        default:
            return new HTTPParser1x0();
    }
}

std::optional<Request> HTTPParser::generateRequest(const std::string& rawRequest) const {
    const std::vector<std::string> requestParts = Utils::split(rawRequest, '\n');
    if(requestParts.size() == 1) {
        return std::nullopt;
    }

    // Request line
    const std::vector<std::string> requestLine = Utils::split(requestParts[0], ' ');
    Endpoint endpoint;
    try{
        endpoint = HTTPController::parseRawEndpoint(requestLine[1]);
    } catch(const std::runtime_error& e) {
        return std::nullopt;
    }
    bool isValidHTTPProtocol = HTTPController::validateHTTPProtocolSyntax(requestLine[2]);
    if(!isValidHTTPProtocol) {
        return std::nullopt;
    }
    requestParts.erase(requestParts.begin());

    // Request headers
    for(auto rawHeader = requestParts.begin(); *rawHeader != ""; rawHeader++ ){

    }
}