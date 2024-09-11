#ifndef CE_HTTPCORE_H
#define CE_HTTPCORE_H

#include "Types.hpp"
#include <map>
#include <vector>
#include <optional>

class HTTPCore {
private:
    Resources resources;
    std::vector<Endpoint> insertOrderedEndpoints;

public:
    void addResourceOperation(Endpoint endpoint, ResourceOperation resourceOperation);
    std::optional<ResourceManager> getResourceManager(Endpoint sysEndpoint, Method sysMethod);
    std::vector<Endpoint> getEndpoints(const unsigned int& numberOfFragments = 0);

};

#endif