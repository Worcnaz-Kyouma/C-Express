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
    const static HTTPParser* httpParser;
    HTTPCore* const httpCore;

    ResourceManager getResourceManager(Endpoint endpoint, const std::string& method) const; //dont forget about HEAD request
public:
    HTTPController(Server* server, AvailableHTTPProtocols protocol);
    
    void addResource(const std::string& method, const std::string& rawEndpoint, ResourceManager resourceManager);
    Process getProcess(const std::string& rawRequest);
};

#endif