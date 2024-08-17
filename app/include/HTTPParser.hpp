#ifndef CE_HTTPPARSER_H
#define CE_HTTPPARSER_H

#include <array>
#include <string>
#include "Types.hpp"

class HTTPParser {
private:
    const static std::array<char*, 2> ENABLED_METHODS; // {'GET', 'POST', 'PATCH', 'DELETE'}

public: 
    Endpoint parseEndpoint(const std::string& rawEndpoint, const std::string& method) const;
    Request parseRequest(const std::string& rawRequest) const;
    Request prepareRequest(Request request, Endpoint endpoint) const;
};

#endif