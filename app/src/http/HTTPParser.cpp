#include "HTTPParser.hpp"
#include "HTTPParser1x0.hpp"
#include "Utils.hpp"
#include <stdexcept>

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
}