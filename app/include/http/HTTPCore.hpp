#ifndef CE_HTTPCORE_H
#define CE_HTTPCORE_H

#include "Types.hpp"
#include <map>
#include <vector>
#include <optional>

class HTTPCore {
private:
    std::map<Endpoint, std::vector<ResourceOperation>> resources;

public:
    void addResource(Resource resource);
    void addResourceOperation(Endpoint endpoint, ResourceOperation resourceOperation);
    std::optional<Resource> getResource(Endpoint endpoint);

};

#endif