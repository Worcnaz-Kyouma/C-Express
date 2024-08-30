#ifndef CE_REQUEST_H
#define CE_REQUEST_H

#include "HTTPController.hpp"
#include <string>

class Request {
private:
    const Endpoint sysEndpoint;
public:
    Request(
        std::string method, 
        std::string endpoint, 
        std::string protocol
    );

    const std::string protocol;
    const std::string method;
    const std::string endpoint;

    const HeadersDStruct headers;
    const ParamsDStruct params;
    const BodyJsonDStruct body;

    friend class HTTPController;
};

#endif