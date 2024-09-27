#ifndef CE_HTTPPARSER1X0_H
#define CE_HTTPPARSER1X0_H

#include "http/HTTPParser.hpp"
#include "http/HTTPController.hpp"
#include <optional>

class HTTPParser1x0 : public HTTPParser {
public:
    HTTPParser1x0(HTTPController* httpControllerHost);

    Method parseMethod(const std::string& method, bool getEmulatedMethod = false) const override;

    Request* generateRequest(const std::string& rawRequest, Socket* clientSocket) const override;
    Response* generateResponse(Request* request) const override;

    std::string parseResponse(Response* response) const override;

    Process getGenericsRM(StatusCode sCode, Socket* clientSocket) const override;
};

#endif