#ifndef CE_HTTPPARSER_H
#define CE_HTTPPARSER_H

#include "Request.hpp"
#include "Response.hpp"
#include <optional>

class HTTPParser {
private:
    HTTPController* const httpControllerHost;

    virtual std::tuple<Method, Endpoint, Protocol> parseRequestLine(const std::string& rawRequestLine) const;
    virtual HeadersDStruct  parseRequestHeaders(const std::vector<std::string>& rawHeadersLines) const;
    virtual QueryDStruct    parseQueryParams(Endpoint endpoint) const;
    virtual ParamsDStruct   parseURLParams(Endpoint endpoint, Endpoint sysEndpoint) const;

    virtual HeadersDStruct  generateResponseHeaders() const;

    virtual bool validateRequest(Request request) const;

    virtual void GRM_NoResourceFound(Request request, Response response);
    virtual void GRM_badRequest(Request request, Response response);
    
protected:
    std::vector<std::string> parseResponseInFields(Response* response) const;

public:
    HTTPParser(HTTPController* httpControllerHost);
    ~HTTPParser();

    static HTTPParser* getHTTPParser(AvailableHTTPProtocols protocol, HTTPController* httpControllerHost);

    virtual Method parseMethod(const std::string& method, bool getEmulatedMethod = false) const = 0;

    virtual Request* generateRequest(const std::string& rawRequest, Socket* clientSocket) const;
    virtual Response* generateResponse(Request* request) const;

    virtual Process getGenericsRM(StatusCode sCode) const;

    virtual std::string parseResponse(Response* response) const = 0;
};

#endif