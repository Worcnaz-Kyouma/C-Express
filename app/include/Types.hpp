#ifndef CE_TYPES_H
#define CE_TYPES_H

#include <utility>
#include <vector>
#include <string>
#include <tuple>
#include <map>
#include "Request.hpp"
#include "Response.hpp"

using Endpoint = std::vector<std::string>;
using Resource = std::pair<Endpoint, std::map<Method, ResourceManager>>;

using Method = std::string;
using ResourceOperation = std::pair<Method, ResourceManager>;
using ResourceManager = void(*)(Request, Response);

using StatusCode = unsigned int;

using Process = std::tuple<ResourceManager, Request, Response>

enum AvailableHTTPProtocols {
    HTTP1x0
};

#endif