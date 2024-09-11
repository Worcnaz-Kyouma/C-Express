#include "HTTPParser1x0.hpp"
#include "Utils.hpp"

#include <algorithm>
#include <optional>
#include <stdexcept>

HTTPParser1x0::HTTPParser1x0(HTTPController* httpControllerHost): HTTPParser(httpControllerHost) {};

Method HTTPParser1x0::parseMethod(const std::string& method, bool getEmulatedMethod = false) const {
    const std::vector<std::string> allowedMethods = {
        "GET",
        "HEAD",
        "POST"
    };

    if(std::find(allowedMethods.begin(), allowedMethods.end(), method) == allowedMethods.end())
        throw new std::runtime_error("Invalid method of HTTP/1.0");
    
    if(getEmulatedMethod){
        if(method == "HEAD") return "GET";
    }

    return method;
}

std::optional<Request> HTTPParser1x0::generateRequest(const std::string& rawRequest) const {
    std::optional<Request> incompleteRequestIt = HTTPParser::generateRequest(rawRequest);
    if(!incompleteRequestIt.has_value()) return std::nullopt;
    Request request = *incompleteRequestIt;

    try{
        this->parseMethod(request.method);
    } catch(const std::runtime_error& e) {
        return std::nullopt;
    }
    if(request.protocol != "HTTP/1.0") return std::nullopt;

    request.isIncomplete = false;

    return request;
}