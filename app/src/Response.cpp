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

// Fix headers logic, user can mess it... Like having many content-type headers with different low or upper case 
void Response::setBodyHeaders(bool isJson) {
    this->headers.erase("Content-Type");
    this->headers.erase("Content-Length");
    
    if(std::holds_alternative<std::string>(this->body)) {
        size_t bodySize = std::get<std::string>(this->body).size();
        
        this->headers.insert({"Content-Length", std::to_string(bodySize)});
        this->headers.insert({"Content-Type", "text/plain"});
    } else {
        size_t bodySize = std::get<json>(this->body).dump().size();

        this->headers.insert({"Content-Length", std::to_string(bodySize)});
        this->headers.insert({"Content-Type", "application/json"});
    }
    
}

void Response::send() {
    this->httpControllerHost->sendResponse(this);
}