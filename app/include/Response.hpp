#ifndef CE_RESPONSE_H
#define CE_RESPONSE_H

#include "Cexpress.hpp"
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
    ~Response();
    
    const Protocol protocol;
    StatusCode statusCode;
    std::string statusDesc;

    HeadersDStruct headers;
    std::variant<json, std::string> body;

    void status(StatusCode newStatus);

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

#endif