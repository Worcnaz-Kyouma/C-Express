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

void Response::send() {
    this->httpControllerHost->sendResponse(this);
}