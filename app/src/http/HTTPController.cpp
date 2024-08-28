#include "HTTPController.hpp"
#include <algorithm>
#include <optional>
#include <stdexcept>

const HTTPParser* HTTPController::httpParser = nullptr;

HTTPController::HTTPController(Server* server, AvailableHTTPProtocols protocol):
server(server),
httpCore(new HTTPCore()) {
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
    switch (response.statusCode){
        case 404:
            rsManager = HTTPController::httpCore->resourceManager404();
            break;
        
        default: // Successfully created request and response without the need of a generic rsManager, and a resource manager exists
            rsManager = *HTTPController::httpParser->getResourceManager(request.endpoint, request.method);
            break;
    }

    return std::make_tuple(request, response, rsManager);
}