#ifndef CE_HTTPPARSER_H
#define CE_HTTPPARSER_H

#include "Request.hpp"
#include "Response.hpp"
#include <optional>

class HTTPParser {
public:
    static HTTPParser* getHTTPParser(AvailableHTTPProtocols protocol);

    virtual bool validateMethod(const std::string& method) const = 0;
    virtual Endpoint parseRawEndpoint(const std::string& rawEndpoint) const;

    virtual Request generateRequest(const std::string& rawRequest) const;
    virtual Response generateResponse(const Request& request) const;
};

#endif