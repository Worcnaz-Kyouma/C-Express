#ifndef CE_TYPES_H
#define CE_TYPES_H

#include <utility>
#include <vector>
#include <string>
#include <tuple>
#include <map>
#include <variant>
#include "Request.hpp"
#include "Response.hpp"

const std::string PROJECT_VERSION = "alpha";

//The order fucking matter
using Endpoint = std::vector<std::string>;
using Method = std::string;
using ResourceManager = void(*)(Request, Response);
using ResourceOperation = std::pair<Method, ResourceManager>;
using ResourceOperations = std::map<Method, ResourceManager>;
using Resource = std::pair<Endpoint, ResourceOperations>;
using Resources = std::map<Endpoint, ResourceOperations>;

using StatusCode = unsigned int;
using StatusDesc = std::string;

using Process = std::tuple<ResourceManager, Request, Response>;

using Protocol = std::string;

using HeadersDStruct = std::map<std::string, std::string>;
using ParamsDStruct = std::map<std::string, std::string>;
using QueryDStruct = std::map<std::string, std::string>;
struct BodyJsonDStruct {
    using AttributeValue = std::variant<std::string, BodyJsonDStruct>;
    std::map<std::string, AttributeValue> dataStructure;
};

enum AvailableHTTPProtocols {
    HTTP1x0
};

#endif