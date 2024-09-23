#ifndef CE_HTTPPARSER_H
#define CE_HTTPPARSER_H

#include "Request.hpp"
#include "Response.hpp"
#include <optional>

class HTTPParser {
private:
    virtual std::tuple<Method, Endpoint, Protocol> parseRequestLine(const std::string& rawRequestLine) const;
    virtual HeadersDStruct  parseRequestHeaders(const std::vector<std::string>& rawHeadersLines) const;
    virtual QueryDStruct    parseQueryParams(Endpoint endpoint) const;
    virtual ParamsDStruct   parseURLParams(Endpoint endpoint, Endpoint sysEndpoint) const;

    virtual bool validateRequest(Request request) const;
    
protected:
    HTTPController* const httpControllerHost;

    virtual HeadersDStruct  generateResponseHeaders() const;
    std::vector<std::string> parseResponseInFields(Response* response) const;

    static void GRM_NoResourceFound(Request* request, Response* response);
    static void GRM_badRequest(Request* request, Response* response);

public:
    HTTPParser(HTTPController* httpControllerHost);

    static HTTPParser* getHTTPParser(AvailableHTTPProtocols protocol, HTTPController* httpControllerHost);

    virtual Method parseMethod(const std::string& method, bool getEmulatedMethod = false) const = 0;

    virtual Request* generateRequest(const std::string& rawRequest, Socket* clientSocket) const;
    virtual Response* generateResponse(Request* request) const;

    virtual std::string parseResponse(Response* response) const = 0;

    virtual Process getGenericsRM(StatusCode sCode, Socket* clientSocket) const = 0;
};

#endif