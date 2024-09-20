#include "Request.hpp"

Request::Request(
    Socket* clientSocket,

    Method method, 
    Endpoint endpoint, 
    Protocol protocol,

    Endpoint sysEndpoint,

    HeadersDStruct headers,
    ParamsDStruct params,
    QueryDStruct query,

    bool isIncomplete = false
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
}

void Request::setBody(json body) {
    this->body = body;
}