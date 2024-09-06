#ifndef CE_REQUEST_H
#define CE_REQUEST_H

#include "HTTPController.hpp"
#include <string>

class Request {
private:
    const Endpoint sysEndpoint;
    bool isIncomplete;
public:
    Request(
        Method method, 
        Endpoint endpoint, 
        Protocol protocol,

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

    friend class HTTPController;
};

#endif