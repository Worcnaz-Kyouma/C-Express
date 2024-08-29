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

ResourceManager HTTPController::getResourceManager(Endpoint endpoint, const std::string& method, bool useGenerics = true) {
    std::optional<Resource> resourceOpt = this->httpCore->getResource(endpoint);
    if(resourceOpt.has_value()) {
        if(useGenerics) return this->httpCore->getGenericRsManager(404);
        return nullptr;
    }
    Resource resource = *resourceOpt;

    try {
        ResourceOperation resourceOperation = resource.second.at(method);
    } catch(const std::out_of_range& e) {
        if(useGenerics) return this->httpCore->getGenericRsManager(501);
    }
}

void HTTPController::addResource(const std::string& method, const std::string& rawEndpoint, ResourceManager resourceManager) {
    bool isValidMethod = HTTPController::httpParser->validateMethod(method);
    if(!isValidMethod) throw std::runtime_error("Invalid method.");

    Endpoint endpoint = HTTPController::httpParser->parseRawEndpoint(rawEndpoint);

    ResourceManager foundRSManager = this->getResourceManager(endpoint, method, false);
    if(foundRSManager != nullptr) throw std::runtime_error("Method already defined to that endpoint.");

    this->httpCore->addResourceOperation(
        endpoint, 
        std::make_pair(Method(method), resourceManager)
    );
}

Process HTTPController::getProcess(const std::string& rawRequest) {
    Request request = HTTPController::httpParser->generateRequest(rawRequest);
    Response response = HTTPController::httpParser->generateResponse(request);
    ResourceManager rsManager = this->getResourceManager(request.sysEndpoint, request.method);

    return std::make_tuple(request, response, rsManager);
}