#include "HTTPParser.hpp"
#include "HTTPParser1x0.hpp"
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

Endpoint parseRawEndpoint(const std::string& rawEndpoint) {
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

    return endpoint;
}

//Implementation methods
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