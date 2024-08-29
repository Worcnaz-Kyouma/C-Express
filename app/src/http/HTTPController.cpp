#include "HTTPController.hpp"
#include <algorithm>
#include <optional>
#include <stdexcept>

const HTTPParser* HTTPController::httpParser = nullptr;

HTTPController::HTTPController(Server* server, AvailableHTTPProtocols protocol):
server(server),
httpCore(new HTTPCore(protocol)) {
    HTTPController::httpParser = HTTPParser::getHTTPParser(protocol);
}

void HTTPController::addResource(const std::string& method, const std::string& rawEndpoint, ResourceManager resourceManager) {
    bool isValidMethod = HTTPController::httpParser->validateMethod(method);
    if(!isValidMethod) throw std::runtime_error("Invalid method.");

    Endpoint endpoint = HTTPController::httpParser->parseRawEndpoint(rawEndpoint);

    std::optional<ResourceManager> foundRSManager = HTTPController::httpParser->getResourceManager(endpoint, method);
    if(foundRSManager != nullptr) throw std::runtime_error("Method already defined to that endpoint.");

    this->httpCore->addResourceOperation(
        endpoint, 
        std::make_pair(Method(method), resourceManager)
    );
}

Process HTTPController::getProcess(const std::string& rawRequest) {
    Request request = HTTPController::httpParser->generateRequest(rawRequest);
    Response response = HTTPController::httpParser->generateResponse(request);
    ResourceManager rsManager = nullptr;

    // If status code already have an value different from 0, it means the request generated a special generic response
    if(response.statusCode != 0) {
        rsManager = this->httpCore->getGenericRsManager(response.statusCode);
    } else {
        rsManager = *HTTPController::httpParser->getResourceManager(request.sysEndpoint, request.method);
    }

    return std::make_tuple(request, response, rsManager);
}