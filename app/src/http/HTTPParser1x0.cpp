#include "http/HTTPParser1x0.hpp"
#include "Utils.hpp"

#include <iostream>
#include <algorithm>
#include <optional>
#include <stdexcept>

HTTPParser1x0::HTTPParser1x0(HTTPController* httpControllerHost): HTTPParser(httpControllerHost) {};

Method HTTPParser1x0::parseMethod(const std::string& method, bool getEmulatedMethod) const {
    const std::vector<std::string> allowedMethods = {
        "GET",
        "HEAD",
        "POST"
    };

    if(std::find(allowedMethods.begin(), allowedMethods.end(), method) == allowedMethods.end())
        throw std::runtime_error("Invalid method of HTTP/1.0");
        
    if(getEmulatedMethod){
        if(method == "HEAD") return "GET";
    }

    return method;
}

Request* HTTPParser1x0::generateRequest(const std::string& rawRequest, Socket* clientSocket) const {
    Request* request = HTTPParser::generateRequest(rawRequest, clientSocket);
    if(request == nullptr) return nullptr;

    try{
        this->parseMethod(request->method);
    } catch(const std::runtime_error& e) {
        return nullptr;
    }
    // if(request->protocol != "HTTP/1.0") return nullptr; Doesn't matter if client dont send the same protocol version as server max, just send the closest possible

    if(request->method == "POST") {
        std::vector<std::string> requestParts = Utils::split(rawRequest, "\r\n\r\n");
        if(requestParts.size() == 2){
            std::string strBody = requestParts[1];
            json body;
            try {
                body = json::parse(strBody);
            } catch (const json::parse_error& e) {
                return nullptr;
            }
            
            request->setBody(body);
        }
    }

    request->isIncomplete = false;
    return request;
}

Response* HTTPParser1x0::generateResponse(Request* request) const {
    Response* response = HTTPParser::generateResponse(request);

    response->protocol = "HTTP/1.0";
    return response;
}

std::string HTTPParser1x0::parseResponse(Response* response) const {
    std::vector<std::string> responseFields = this->parseResponseInFields(response);
    if(response->requestOrigin->method == "HEAD") {
        return responseFields[0] + "\r\n" + responseFields[1] + "\r\n" + "\r\n";
    }
    
    std::cout << "End response str: " << std::endl;
    std::cout << responseFields[0] + "\r\n" + responseFields[1] + "\r\n" + "\r\n" + responseFields[2] << std::endl;
    std::cout << std::endl;

    return responseFields[0] + "\r\n" + responseFields[1] + "\r\n" + "\r\n" + responseFields[2];
}

Process HTTPParser1x0::getGenericsRM(StatusCode sCode, Socket* clientSocket) const {
    Request* genericReq = new Request(
        clientSocket,
        Method("GET"),
        Endpoint({"/"}),
        Protocol("HTTP/1.0"),

        Endpoint({"/"})
    );

    HeadersDStruct resHeaders = this->generateResponseHeaders();
    Response* genericRes = new Response(
        genericReq,
        this->httpControllerHost,
        Protocol("HTTP/1.0"),
        500,
        "Internal Server Error",
        resHeaders
    );

    ResourceManager rsManager;
    switch(sCode) {
        case 400:
            rsManager = &HTTPParser1x0::GRM_badRequest;
            break;
        case 404:
            rsManager = &HTTPParser1x0::GRM_NoResourceFound;
            break;
    }

    return {
        rsManager,
        genericReq,
        genericRes
    };
}