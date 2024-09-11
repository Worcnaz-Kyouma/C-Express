#ifndef CE_HTTPPARSER1X0_H
#define CE_HTTPPARSER1X0_H

#include "HTTPParser.hpp"
#include "HTTPController.hpp"
#include <optional>

class HTTPParser1x0 : public HTTPParser {
public:
    HTTPParser1x0(HTTPController* httpControllerHost);

    Method parseMethod(const std::string& method, bool getEmulatedMethod = false) const override;

    std::optional<Request> generateRequest(const std::string& rawRequest) const override;
};

#endif