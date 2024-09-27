#ifndef CE_RESPONSE_H
#define CE_RESPONSE_H

#include "Types.hpp"
class Request;
#include "Request.hpp"
class HTTPController;
#include "http/HTTPController.hpp"

#include <variant>
#include <type_traits>

class Response {
private:
    const Request* const requestOrigin;
    HTTPController* const httpControllerHost;

    void setBodyHeaders(bool isJson = false);
public:
    Response(
        Request* requestOrigin,
        HTTPController* httpControllerHost,
        
        Protocol protocol,
        StatusCode statusCode, 
        std::string statusDesc,

        HeadersDStruct headers
    );
    
    Protocol protocol;
    StatusCode statusCode;
    std::string statusDesc;

    HeadersDStruct headers;
    std::variant<json, std::string> body;

    void status(StatusCode newStatus);

    //Change to receive always text with a boolean is json! Cause like this we can preserve order with json_ordered, cause default nlohmann json is not ordered
    template <typename T>
    typename std::enable_if<std::is_same<T, std::string>::value || std::is_same<T, json>::value, void>::type
    setBody(T body);
    
    template <typename T>
    typename std::enable_if<std::is_same<T, std::string>::value || std::is_same<T, json>::value, void>::type
    send(T body);

    void send(); //use HTTP Controller with HTTP Parser to mantain structure logic with http protocol

    friend class HTTPController;
    friend class HTTPParser1x0;
};

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

#endif