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

    headers(headers){}

Response::~Response() {
    delete this->requestOrigin;
    delete this->httpControllerHost;
}

void Response::status(StatusCode newStatus) {
    auto [ statusCode, statusDesc ] = this->httpControllerHost->getHTTPStatus(newStatus);

    this->statusCode = statusCode;
    this->statusDesc = statusDesc;
}

void Response::setBodyHeaders(bool isJson) {
    this->headers.erase("content-type");
    this->headers.erase("content-length");
    
    if(std::holds_alternative<std::string>(this->body)) {
        size_t bodySize = std::get<std::string>(this->body).size();
        
        this->headers.insert({"content-length", std::to_string(bodySize)});
        this->headers.insert({"content-type", "plain/text"});
    } else {
        size_t bodySize = std::get<json>(this->body).dump().size();

        this->headers.insert({"content-length", std::to_string(bodySize)});
        this->headers.insert({"content-type", "application/json"});
    }
    
}

template <typename T>
typename std::enable_if<std::is_same<T, std::string>::value || std::is_same<T, json>::value, void>::type
Response::setBody(T body) {
    this->body = body;
    this->setBodyHeaders();
}

template <typename T>
typename std::enable_if<std::is_same<T, std::string>::value || std::is_same<T, json>::value, void>::type
Response::send(T body) {
    this->setBody(body);

    this->send();
}

void Response::send() {
    this->httpControllerHost->sendResponse(this);
}