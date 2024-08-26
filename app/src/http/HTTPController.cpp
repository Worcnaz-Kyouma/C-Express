#include "HTTPController.hpp"
#include <algorithm>
#include <stdexcept>

const HTTPParser* HTTPController::httpParser = nullptr;

HTTPController::HTTPController(Server* server, AvailableHTTPProtocols protocol):
server(server),
httpCore(new HTTPCore()) {
    HTTPController::httpParser = HTTPParser::getHTTPParser(protocol);
}

void HTTPController::addResource(const std::string& method, const std::string& rawEndpoint, ResourceManager resourceManager) {
    bool isValidMethod = HTTPParser::validateMethod(method);
    if(!isValidMethod) throw std::runtime_error("Invalid method.");

    Endpoint endpoint = HTTPParser::parseRawEndpoint(rawEndpoint);
}