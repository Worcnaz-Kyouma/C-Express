#ifndef CE_HTTPPARSER_H
#define CE_HTTPPARSER_H

#include "Request.hpp"
#include "Response.hpp"
#include <optional>

class HTTPParser {
private:
    virtual void GRM_NoResourceFound(Request request, Response response);
    virtual void GRM_NoResourceOperation(Request request, Response response);
public:
    static HTTPParser* getHTTPParser(AvailableHTTPProtocols protocol);

    virtual Method parseMethod(const std::string& method) const = 0;
    virtual Endpoint parseRawEndpoint(const std::string& rawEndpoint) const;

    virtual Request generateRequest(const std::string& rawRequest) const;
    virtual Response generateResponse(const Request& request) const;

    virtual ResourceManager getGenericsRM(StatusCode sCode) const;
};

#endif