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

ResourceManager HTTPController::getResourceManager(Endpoint endpoint, const std::string& method) const {
    std::optional<Resource> resourceOpt = this->httpCore->getResource(endpoint);
    if(!resourceOpt.has_value()) return nullptr;

    Resource resource = *resourceOpt;

    try {
        ResourceOperation resourceOperation = resource.second.at(method);

        ResourceManager resourceManager = resourceOperation.second;
    } catch(const std::out_of_range& e) {
        return nullptr;
    }
}

void HTTPController::addResource(const std::string& rawMethod, const std::string& rawEndpoint, ResourceManager resourceManager) {
    Method method = HTTPController::httpParser->parseMethod(rawMethod);

    Endpoint endpoint = HTTPController::httpParser->parseRawEndpoint(rawEndpoint);

    ResourceManager foundRSManager = this->getResourceManager(endpoint, method);
    if(foundRSManager != nullptr) throw std::runtime_error("Method already defined to that endpoint.");

    this->httpCore->addResourceOperation(
        endpoint, 
        std::make_pair(Method(method), resourceManager)
    );
}

Process HTTPController::getProcess(const std::string& rawRequest) {
    Request request = HTTPController::httpParser->generateRequest(rawRequest);
    Response response = HTTPController::httpParser->generateResponse(request);

    ResourceManager rsManager = this->getResourceManager(
        request.sysEndpoint, 
        this->httpParser->parseMethod(request.method)
    );
    if(rsManager == nullptr) {
        rsManager = this->httpParser->getGenericsRM(response.statusCode);
    }

    return std::make_tuple(request, response, rsManager);
}