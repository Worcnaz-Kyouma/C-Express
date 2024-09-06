#ifndef CE_HTTPCONTROLLER_H
#define CE_HTTPCONTROLLER_H

#include "Types.hpp"
#include "Server.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "HTTPParser.hpp"
#include "HTTPCore.hpp"

#include <tuple>

class HTTPController {
private:
    Server* const server;
    HTTPParser* const httpParser;
    HTTPCore* const httpCore;

    ResourceManager getResourceManager(Endpoint endpoint, const std::string& method) const; //dont forget about HEAD request
public:
    HTTPController(Server* server, AvailableHTTPProtocols protocol);

    static Endpoint parseRawEndpoint(const std::string& rawEndpoint);
    static Protocol parseRawProtocol(const std::string& rawProtocol);
    static bool validateHTTPHeaderNameSyntax(const std::string& headerName);
    
    Endpoint getSysEndpoint(Endpoint sourceEndpoint);
    void addResource(const std::string& method, const std::string& rawEndpoint, ResourceManager resourceManager);
    Process getProcess(const std::string& rawRequest);
};

#endif