#include <string>
#include <vector>
#include <iostream>
#include "Response.hpp"
#include "Request.hpp"

Response::Response(
    Request* requestOrigin,
    HTTPController* httpController,

    Protocol protocol,
    StatusCode statusCode,
    std::string statusDesc,

    HeadersDStruct headers
):
    requestOrigin(requestOrigin),
    httpControllerHost(httpController),
    
    protocol(protocol),
    statusCode(statusCode),
    statusDesc(statusDesc),

    headers(headers)
{}

void Response::status(StatusCode newStatus) {
    auto [ statusCode, statusDesc ] = this->httpControllerHost->getHTTPStatus(newStatus);

    this->statusCode = statusCode;
    this->statusDesc = statusDesc;
}

template <typename T>
typename std::enable_if<std::is_same<T, std::string>::value || std::is_same<T, JsonDStruct>::value, void>::type
Response::setBody(T body) {
    this->body = body;
}

template <typename T>
typename std::enable_if<std::is_same<T, std::string>::value || std::is_same<T, JsonDStruct>::value, void>::type
Response::send(T body) {
    this->setBody(body);

    this->send();
}

void Response::send() {
    this->httpControllerHost->sendResponse(this);
}