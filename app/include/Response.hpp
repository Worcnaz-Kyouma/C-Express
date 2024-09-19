#ifndef CE_RESPONSE_H
#define CE_RESPONSE_H

#include "Request.hpp"
#include "Types.hpp"
#include <variant>
#include <type_traits>

class Response {
private:
    const Request* const requestOrigin;
    HTTPController* const httpControllerHost;
public:
    Response(
        Request* requestOrigin,
        HTTPController* httpControllerHost,
        
        Protocol protocol,
        StatusCode statusCode, 
        std::string statusDesc,

        HeadersDStruct headers
    );
    
    const Protocol protocol;
    StatusCode statusCode;
    std::string statusDesc;

    const HeadersDStruct headers;
    std::variant<JsonDStruct, std::string> body;

    void status(StatusCode newStatus);

    template <typename T>
    typename std::enable_if<std::is_same<T, std::string>::value || std::is_same<T, JsonDStruct>::value, void>::type
    setBody(T body);
    
    template <typename T>
    typename std::enable_if<std::is_same<T, std::string>::value || std::is_same<T, JsonDStruct>::value, void>::type
    send(T body);

    void send(); //use HTTP Controller with HTTP Parser to mantain structure logic with http protocol

    friend class HTTPController;
    friend class HTTPParser1x0;
};

#endif