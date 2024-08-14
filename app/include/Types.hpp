#ifndef CE_TYPES_H
#define CE_TYPES_H

#include <vector>
#include <string>
#include "Request.hpp"
#include "Response.hpp"

using Process = void(*)(Request, Response);
using Endpoint = std::vector<std::string>;

#endif