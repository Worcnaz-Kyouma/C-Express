#ifndef CE_RESPONSE_H
#define CE_RESPONSE_H

#include "Socket.hpp"

class Response {
private:
    Server* server;
public:
    Response(Server* server);
    StatusCode statusCode;

    void send(const std::string& message); //use HTTP Controller with HTTP Parser to mantain structure logic with http protocol

    friend class HTTPController;
};

#endif