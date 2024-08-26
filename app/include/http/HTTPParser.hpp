#ifndef CE_HTTPPARSER_H
#define CE_HTTPPARSER_H

#include "Request.hpp"
#include "Response.hpp"

class HTTPParser {
public:
    static HTTPParser* getHTTPParser(AvailableHTTPProtocols protocol);

    virtual Request* generateRequest();
    virtual Response* generateResponse();
};

#endif