#include "Request.hpp"

Request::Request(
    std::string method, 
    std::string endpoint, 
    std::string protocol
): 
    method(method),
    endpoint(endpoint),
    protocol(protocol) {};

Request& Request::operator=(const Request $other) {
    return *this;
}