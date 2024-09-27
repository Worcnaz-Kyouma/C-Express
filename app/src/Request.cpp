#include "Request.hpp"
#include <iostream>

Request::Request(
    Socket* clientSocket,

    Method method, 
    Endpoint endpoint, 
    Protocol protocol,

    Endpoint sysEndpoint,

    HeadersDStruct headers,
    ParamsDStruct params,
    QueryDStruct query,

    bool isIncomplete
): 
    clientSocket(clientSocket),
    method(method),
    endpoint(endpoint),
    protocol(protocol),
    sysEndpoint(sysEndpoint),
    headers(headers),
    params(params),
    query(query) {}

Request::~Request() {
    delete this->clientSocket;
    this->clientSocket = nullptr;
}

void Request::setBody(json body) {
    this->body = body;
}