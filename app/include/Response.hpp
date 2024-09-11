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

    void status(StatusCode newStatus);
    void body(const std::string& bodyMessage);
    void body(BodyJsonDStruct bodyJSON);
    
    void send(const std::string& bodyMessage);
    void send(BodyJsonDStruct bodyJSON);
    void send(); //use HTTP Controller with HTTP Parser to mantain structure logic with http protocol

    friend class HTTPController;
};

#endif