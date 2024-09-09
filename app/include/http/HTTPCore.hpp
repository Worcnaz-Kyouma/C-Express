#ifndef CE_HTTPCORE_H
#define CE_HTTPCORE_H

#include "Types.hpp"
#include <map>
#include <vector>
#include <optional>

class HTTPCore {
private:
    std::map<Endpoint, std::map<Method, ResourceManager>> resources;

public:
    void addResource(Resource resource);
    void addResourceOperation(Endpoint endpoint, ResourceOperation resourceOperation);
    std::optional<Resource> getResource(Endpoint endpoint);
    std::optional<ResourceManager> getResourceManager(Endpoint endpoint, Method sysMethod);
    std::vector<Endpoint> getEndpoints(const unsigned int& numberOfFragments = 0);

};

#endif