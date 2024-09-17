#ifndef CE_REQUEST_H
#define CE_REQUEST_H

#include "HTTPController.hpp"
#include <string>

class Request {
private:
    const Socket* clientSocket;
    const Endpoint sysEndpoint;
    bool isIncomplete;
public:
    Request(
        Socket* clientSocket,

        Method method, 
        Endpoint endpoint, 
        Protocol protocol,

        Endpoint sysEndpoint,

        HeadersDStruct headers,
        ParamsDStruct params,
        QueryDStruct query,

        bool isIncomplete = false
    );

    const Method method;
    const std::string endpoint; //Real endpoint, not the sys
    const Protocol protocol;

    const HeadersDStruct headers;
    const ParamsDStruct params;
    const QueryDStruct query;
    const BodyJsonDStruct body;

    void setBody(BodyJsonDStruct body);

    friend class HTTPController;
    friend class HTTPParser;
    friend class HTTPParser1x0;
};

#endif