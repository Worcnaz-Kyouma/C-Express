#ifndef CE_REQUEST_H
#define CE_REQUEST_H

#include "http/HTTPController.hpp"
#include <string>

class Request {
private:
    Socket* clientSocket;
    const Endpoint sysEndpoint;
    bool isIncomplete;
public:
    Request(
        Socket* clientSocket,

        Method method, 
        Endpoint endpoint, 
        Protocol protocol,

        Endpoint sysEndpoint,

        HeadersDStruct headers = HeadersDStruct(),
        ParamsDStruct params = ParamsDStruct(),
        QueryDStruct query = QueryDStruct(),

        bool isIncomplete = false
    );
    ~Request();

    const Method method;
    const Endpoint endpoint; //Real endpoint, not the sys
    const Protocol protocol;

    HeadersDStruct headers;
    const ParamsDStruct params;
    const QueryDStruct query;
    json body;

    void setBody(json body);

    friend class HTTPController;
    friend class HTTPParser;
    friend class HTTPParser1x0;
};

#endif