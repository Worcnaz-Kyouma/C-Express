#ifndef CE_HTTPCORE_H
#define CE_HTTPCORE_H

#include "Types.hpp"
#include "HTTPGenericsRM.hpp"
#include <map>
#include <vector>

class HTTPCore {
private:
    std::map<Endpoint, std::vector<ResourceOperation>> resources;
    HTTPGenericsRM* const grm;

public:
    HTTPCore(AvailableHTTPProtocols protocol);

    void addResource(Resource resource);
    void addResourceOperation(Endpoint endpoint, ResourceOperation resourceOperation);
    Resource getResource(Endpoint endpoint);

    ResourceManager getGenericRsManager(unsigned int statusCode);

};

#endif