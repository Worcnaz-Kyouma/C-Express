#ifndef CE_RESPONSE_H
#define CE_RESPONSE_H

#include "Socket.hpp"

class Response {
private:
    HTTPController* httpControllerHost;
public:
    Response(
        HTTPController* httpControllerHost,
        
        Protocol protocol,
        StatusCode method, 
        std::string statusDesc,

        HeadersDStruct headers
    );
    
    const Protocol protocol;
    StatusCode statusCode;
    std::string statusDesc;

    const HeadersDStruct headers;
    const BodyJsonDStruct body;

    void send(const std::string& message); //use HTTP Controller with HTTP Parser to mantain structure logic with http protocol

    friend class HTTPController;
};

#endif