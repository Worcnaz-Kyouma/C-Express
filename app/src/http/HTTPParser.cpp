#include "http/HTTPParser.hpp"
#include "http/HTTPParser1x0.hpp"
#include "http/HTTPController.hpp"
#include "Utils.hpp"

#include <string>
#include <variant>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iostream>
#include <iomanip>

// Implementation functions
std::string getCurrentTimeFormatted();

HTTPParser::HTTPParser(HTTPController* httpControllerHost): httpControllerHost(httpControllerHost) {};

HTTPParser* HTTPParser::getHTTPParser(AvailableHTTPProtocols protocol, HTTPController* httpControllerHost) {
    switch (protocol){
        case HTTP1x0:
            return new HTTPParser1x0(httpControllerHost);
        default:
            return new HTTPParser1x0(httpControllerHost);
    }
}

std::tuple<Method, Endpoint, Protocol> HTTPParser::parseRequestLine(const std::string& rawRequestLine) const {
    const std::vector<std::string> requestLine = Utils::split(rawRequestLine, " ");
    if(requestLine.size() != 3){
        throw std::runtime_error("Invalid string request line");
    }
    Method unverifiedMethod = requestLine[0];
    Endpoint endpoint;
    Protocol protocol;

    try{
        endpoint = HTTPController::parseRawEndpoint(requestLine[1]);
        protocol = HTTPController::parseRawProtocol(requestLine[2]);
    } catch(const std::runtime_error& e) {
        throw std::runtime_error("Invalid string endpoint or protocol structure");
    }

    return { unverifiedMethod, endpoint, protocol };
}

// All key maps need to be lowercase
HeadersDStruct HTTPParser::parseRequestHeaders(const std::vector<std::string>& rawHeadersLines) const {
    HeadersDStruct headers;
    
    // OBS1: OLD HTTP1.0 allowed multiple lines header field, that got deprecated by unsafe and inconsistent concept 
    // Header field-> header-identifier: header-value\r\n
    for(auto rawHeader = rawHeadersLines.begin(); rawHeader != rawHeadersLines.end(); rawHeader++ ){
        std::vector<std::string> headerParts = Utils::split(*rawHeader, ":", true);
        if(headerParts.size() != 2) {
            throw std::runtime_error("Invalid header structure");
        } 

        std::string headerName = Utils::toLower(headerParts[0]);
        std::string headerValue = headerParts[1];
        if(headerValue[0] != ' ') {
            throw std::runtime_error("Invalid header structure");
        }
        headerValue.erase(headerValue.begin()); // Remove blank line
        
        bool isValidHeader = HTTPController::validateHTTPHeaderNameSyntax(headerName);
        if(!isValidHeader) throw std::runtime_error("Invalid header name syntax");

        headers.insert({headerName, headerValue});
    }

    return headers;
}

QueryDStruct HTTPParser::parseQueryParams(Endpoint endpoint) const {
    QueryDStruct query;
    
    std::string lastFragEP = endpoint.back();
    if(lastFragEP.find('?') != std::string::npos) {
        std::vector<std::string> lastFragEPQuestionMarkSplitted = Utils::split(lastFragEP, "?", true);
        std::string queryString = lastFragEPQuestionMarkSplitted.back();

        std::vector<std::string> queryParts = Utils::split(queryString, "&");
        for(auto rawQueryParam = queryParts.begin(); rawQueryParam != queryParts.end(); rawQueryParam++) {
            std::vector<std::string> queryParam = Utils::split(*rawQueryParam, "=");
            
            std::string queryParamIdentifier = queryParam[0];
            std::string queryParamValue = queryParam[1];

            query.insert({queryParamIdentifier, queryParamValue});
        }
    }

    return query;
}

ParamsDStruct HTTPParser::parseURLParams(Endpoint endpoint, Endpoint sysEndpoint) const {
    ParamsDStruct params;

    for(auto endpointFragment = sysEndpoint.begin(); endpointFragment != sysEndpoint.end(); endpointFragment++){
        if(*(endpointFragment->begin()) == ':'){
            std::string paramIdentifier = *endpointFragment;
            paramIdentifier.erase(paramIdentifier.begin()); //Remove the starting ':'

            auto paramValueEndpointIndex = std::distance(sysEndpoint.begin(), endpointFragment);
            std::string paramValue = endpoint[paramValueEndpointIndex];

            params.insert({paramIdentifier, paramValue});
        }
    }

    return params;
}

bool HTTPParser::validateRequest(Request* request) const {
    // Content-Type must be "application/json" if exists, due to C-Express limitation
    auto headerFieldIt = request->headers.find("content-type");
    if(headerFieldIt != request->headers.end())
        if(Utils::toLower(headerFieldIt->second) != "application/json") 
            return false;

    return true;
}

// Maybe change that mother fucker, to return a request if works, and a status code if error
Request* HTTPParser::generateRequest(const std::string& rawRequest, Socket* clientSocket) const {
    std::cout << "Request: " << std::endl << rawRequest << std::endl << std::endl;
    std::cout << "Request int: " << std::endl;
    std::for_each(rawRequest.begin(), rawRequest.end(), [](const char& c) {
        std::cout << static_cast<int>(c) << " ";
    });
    std::cout << std::endl;
    
    std::vector<std::string> requestParts = Utils::split(rawRequest, "\r\n");
    if(requestParts.size() == 1) {
        return nullptr;
    }

    // Fine
    for(int c = 0; c<requestParts.size(); c++) {
        std::cout << "Request Part " << c << ": " << requestParts[c] << std::endl;
    }
    std::cout << std::endl;

    std::vector<std::string> rawHeadersLines;
    
    auto emptyLine = std::find(requestParts.begin()+1, requestParts.end(), "");
    if(emptyLine == requestParts.end()) return nullptr;

    std::copy(requestParts.begin()+1, emptyLine, std::back_inserter(rawHeadersLines));

    // Fine
    for(int c = 0; c<rawHeadersLines.size(); c++) {
        std::cout << "Req Header " << c << ": " << rawHeadersLines[c] << std::endl;
    }
    std::cout << std::endl;

    try{
        auto [ unverifiedMethod, endpoint, protocol ] = this->parseRequestLine(requestParts[0]);
        std::cout << unverifiedMethod << std::endl;
        std::cout << Utils::join(endpoint, "/") << std::endl;
        std::cout << protocol << std::endl;
        std::cout << std::endl;

        std::optional<Endpoint> sysEndpointOpt = httpControllerHost->getSysEndpoint(endpoint);
        if(!sysEndpointOpt.has_value()) return nullptr; //We could improve that, returning a error here with 404
        std::cout << "sysEndpoint: " << Utils::join(*sysEndpointOpt, "/") << std::endl;
        std::cout << std::endl;

        HeadersDStruct headers = this->parseRequestHeaders(rawHeadersLines);
        for(const auto& header : headers) {
            std::cout << "Header name: " << header.first << std::endl;
            std::cout << "Header value: " << header.second << std::endl;
        }
        std::cout << std::endl;

        QueryDStruct query = this->parseQueryParams(endpoint);
        for(const auto& queryP : query) {
            std::cout << "Query param name: " << queryP.first << std::endl;
            std::cout << "Query param value: " << queryP.second << std::endl;
        }
        std::cout << std::endl;

        ParamsDStruct params = this->parseURLParams(endpoint, *sysEndpointOpt);
        for(const auto& param : params) {
            std::cout << "Param name: " << param.first << std::endl;
            std::cout << "Param value: " << param.second << std::endl;
        }
        std::cout << std::endl;

        Request* incompleteRequest = new Request(
            clientSocket,

            unverifiedMethod,
            endpoint,
            protocol,

            *sysEndpointOpt,

            headers,
            query,
            params,
            true
        );

        bool isRequestValid = this->validateRequest(incompleteRequest);
        if(!isRequestValid) return nullptr;

        std::cout << "Created... D A M N" << std::endl;
        std::cout << std::endl;

        return incompleteRequest;
    } catch(const std::runtime_error& e) {
        return nullptr;
    }
}

HeadersDStruct HTTPParser::generateResponseHeaders() const {
    HeadersDStruct headers;

    // Date header
    std::string timeHeaderName = "Date";
    std::string timeHeaderValue = getCurrentTimeFormatted();
    headers.insert({timeHeaderName, timeHeaderValue});

    //Server header
    std::string serverHeaderName = "Server";
    std::string serverHeaderValue = "C-Express/" + PROJECT_VERSION + " - By Worcnaz Kyouma";
    headers.insert({serverHeaderName, serverHeaderValue});

    return headers;
}

Response* HTTPParser::generateResponse(Request* request) const{
    HeadersDStruct headers = this->generateResponseHeaders();
    for(const auto& header : headers) {
        std::cout << "Res Header name: " << header.first << std::endl;
        std::cout << "Res Header value: " << header.second << std::endl;
    }
    std::cout << std::endl;
    
    Response* response = new Response(
        request,
        this->httpControllerHost,

        request->protocol,
        200,
        "OK",

        headers
    );

    return response;
}

std::vector<std::string> HTTPParser::parseResponseInFields(Response* response) const{
    std::string statusLine = response->protocol + " " + std::to_string(response->statusCode) + " " + response->statusDesc;

    std::vector<std::string> vecHeaders;
    std::transform(response->headers.begin(), response->headers.end(), std::back_inserter(vecHeaders), [](const std::pair<std::string, std::string>& header) {
        std::string strHeader = header.first + ": " + header.second;
        return strHeader;
    });
    std::string headers = Utils::join(vecHeaders, "\n\r");

    std::string body;
    if(std::holds_alternative<std::string>(response->body)) {
        body = std::get<std::string>(response->body);
    } else {
        body = std::get<json>(response->body).dump();
    }

    return {
        statusLine,
        headers,
        body
    };
}

void HTTPParser::GRM_NoResourceFound(Request* request, Response* response) {
    response->status(404);

    response->setBody<std::string>("Resource not found. Sorry mister client");

    response->send();
}

void HTTPParser::GRM_badRequest(Request* request, Response* response) {
    response->status(400);

    response->setBody<std::string>("Bro, i didn't get a clue what you are talking about, learn to talk HTTP!");

    response->send();
}

// Implementation functions
std::string getCurrentTimeFormatted() {
    auto now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
    std::tm* nowTimeStruct = std::gmtime(&nowTime);
    std::ostringstream oss;
    oss << std::put_time(nowTimeStruct, "%a %d %b %Y %H:%M:%S GMT");

    return oss.str();
}