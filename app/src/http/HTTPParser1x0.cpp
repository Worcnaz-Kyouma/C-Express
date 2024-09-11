#include "HTTPParser1x0.hpp"

HTTPParser1x0::HTTPParser1x0(HTTPController* httpControllerHost): HTTPParser(httpControllerHost) {};

Method HTTPParser1x0::parseMethod(const std::string& method, bool getEmulatedMethod = false) const {

}

std::optional<Request> HTTPParser1x0::generateRequest(const std::string& rawRequest) const {
    
}