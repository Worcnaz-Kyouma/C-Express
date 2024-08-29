#ifndef CE_REQUEST_H
#define CE_REQUEST_H

#include "HTTPController.hpp"
#include <string>

class Request {
private:
    const std::string protocol;
    const Endpoint sysEndpoint;
public:
    Request(
        std::string method, 
        std::string endpoint, 
        std::string protocol
    );

    const std::string method;
    const std::string endpoint; //Return without the infront method 

    Request& operator=(const Request $other);

    friend class HTTPController;
};

#endif