#include "HTTPParser.hpp"
#include "HTTPParser1x0.hpp"
#include "HTTPController.hpp"
#include "Utils.hpp"
#include <stdexcept>
#include <algorithm>

HTTPParser* HTTPParser::getHTTPParser(AvailableHTTPProtocols protocol, HTTPController* httpControllerHost) {
    switch (protocol){
        case HTTP1x0:
            return new HTTPParser1x0(httpControllerHost);
        default:
            return new HTTPParser1x0(httpControllerHost);
    }
}

std::tuple<Method, Endpoint, Protocol> HTTPParser::parseRequestLine(const std::string& rawRequestLine) const {
    const std::vector<std::string> requestLine = Utils::split(rawRequestLine, ' ');
    if(requestLine.size() != 3){
        throw std::runtime_error("Invalid string request line");
    }
    Method unverifiedMethod = requestLine[0];
    Endpoint endpoint;
    Protocol unvalidatedProtocol;

    try{
        endpoint = HTTPController::parseRawEndpoint(requestLine[1]);
        unvalidatedProtocol = HTTPController::parseRawProtocol(requestLine[2]);
    } catch(const std::runtime_error& e) {
        throw std::runtime_error("Invalid string endpoint or protocol structure");
    }

    return { unverifiedMethod, endpoint, unvalidatedProtocol };
}

HeadersDStruct HTTPParser::parseRequestHeaders(const std::vector<std::string>& rawHeadersLines) const {
    HeadersDStruct headers;
    
    // OBS1: OLD HTTP1.0 allowed multiple lines header field, that got deprecated by unsafe and inconsistent concept 
    // Header field-> header-identifier: header-value\r\n
    for(auto rawHeader = rawHeadersLines.begin(); rawHeader != rawHeadersLines.end(); rawHeader++ ){
        std::vector<std::string> headerParts = Utils::split(*rawHeader, ':', true);
        if(headerParts.size() != 2) {
            throw std::runtime_error("Invalid header structure");
        } 

        std::string headerName = headerParts[0];
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
        std::vector<std::string> lastFragEPQuestionMarkSplitted = Utils::split(lastFragEP, '?', true);
        std::string queryString = lastFragEPQuestionMarkSplitted.back();

        std::vector<std::string> queryParts = Utils::split(queryString, '&');
        for(auto rawQueryParam = queryParts.begin(); rawQueryParam != queryParts.end(); rawQueryParam++) {
            std::vector<std::string> queryParam = Utils::split(*rawQueryParam, '=');
            
            std::string queryParamIdentifier = queryParam[0];
            std::string queryParamValue = queryParam[1];

            query.insert({queryParamIdentifier, queryParamValue});
        }
    }

    return query;
}

ParamsDStruct HTTPParser::parseURLParams(Endpoint endpoint) const {
    ParamsDStruct params;
    std::optional<Endpoint> sysEndpointOpt = httpControllerHost->getSysEndpoint(endpoint);
    if(!sysEndpointOpt.has_value()) throw std::runtime_error("No defined endpoint found");

    Endpoint sysEndpoint = *sysEndpointOpt;
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

std::optional<Request> HTTPParser::generateRequest(const std::string& rawRequest) const {
    std::vector<std::string> requestParts = Utils::split(rawRequest, '\r\n');
    if(requestParts.size() == 1) {
        return std::nullopt;
    }

    try{
        auto [ unverifiedMethod, endpoint, unvalidatedProtocol ] = this->parseRequestLine(requestParts[0]);

        auto emptyLine = std::find(requestParts.begin()+1, requestParts.end(), "");
        const std::vector<std::string> rawHeadersLines;
        std::copy(requestParts.begin(), emptyLine, rawHeadersLines);

        HeadersDStruct headers = this->parseRequestHeaders(rawHeadersLines);
        QueryDStruct query = this->parseQueryParams(endpoint);
        ParamsDStruct params = this->parseURLParams(endpoint);

        Request incompleteRequest(
            unverifiedMethod,
            endpoint,
            unvalidatedProtocol,

            headers,
            query,
            params,
            true
        );

        return incompleteRequest;
    } catch(const std::runtime_error& e) {
        return std::nullopt;
    }
}