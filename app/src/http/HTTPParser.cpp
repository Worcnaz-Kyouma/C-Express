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

std::optional<Request> HTTPParser::generateRequest(const std::string& rawRequest) const {
    std::vector<std::string> requestParts = Utils::split(rawRequest, '\r\n');
    if(requestParts.size() == 1) {
        return std::nullopt;
    }

    // Request line
    const std::vector<std::string> requestLine = Utils::split(requestParts[0], ' ');
    if(requestLine.size() != 3){
        return std::nullopt;
    }
    Method unverifiedMethod = requestLine[0];
    Endpoint endpoint;
    Protocol unvalidatedProtocol;

    try{
        endpoint = HTTPController::parseRawEndpoint(requestLine[1]);
        unvalidatedProtocol = HTTPController::parseRawProtocol(requestLine[2]);
    } catch(const std::runtime_error& e) {
        return std::nullopt;
    }

    requestParts.erase(requestParts.begin());

    // Request headers 
    HeadersDStruct headers;
    // OBS1: OLD HTTP1.0 allowed multiple lines header field, that got deprecated by unsafe and inconsistent concept 

    // Header field-> header-identifier: header-value\r\n
    for(auto rawHeader = requestParts.begin(); *rawHeader != ""; rawHeader++ ){
        std::vector<std::string> headerParts = Utils::split(*rawHeader, ':', true);
        if(headerParts.size() != 2) {
            return std::nullopt;
        } 

        std::string headerName = headerParts[0];
        std::string headerValue = headerParts[1];
        if(headerValue[0] != ' ') {
            return std::nullopt;
        }
        headerValue.erase(headerValue.begin()); // Remove blank line
        
        bool isValidHeader = HTTPController::validateHTTPHeaderNameSyntax(headerName);
        if(!isValidHeader) return std::nullopt;

        headers.insert({headerName, headerValue});
    }

    // Query params
    QueryDStruct query;
    std::string lastFragEP = endpoint.back();
    if(lastFragEP.find('?') != std::string::npos) {
        std::vector<std::string> lastFragEPQuestionMarkSplitted = Utils::split(lastFragEP, '?', true);
        std::string queryString = lastFragEPQuestionMarkSplitted.back();

        std::vector<std::string> queryParts = Utils::split(queryString, '&');
        for(auto rawQueryParam = queryParts.begin(); rawQueryParam != queryParts.end(); rawQueryParam++) {
            std::vector<std::string> queryParam = Utils::split(*rawQueryParam, '=');
            
            std::string queryPIdentifier = queryParam[0];
            std::string queryPValue = queryParam[1];

            query.insert({queryPIdentifier, queryPValue});
        }
    }

    // URL params
    ParamsDStruct params;
    Endpoint sysEndpoint = httpControllerHost->getSysEndpoint(endpoint);
    for(auto endpointFragment = sysEndpoint.begin(); endpointFragment != sysEndpoint.end(); endpointFragment++){
        if(*(endpointFragment->begin()) == ':'){
            std::string paramIdentifier = *endpointFragment;
            paramIdentifier.erase(paramIdentifier.begin()); //Remove the starting ':'

            auto paramValueEndpointIndex = std::distance(sysEndpoint.begin(), endpointFragment);
            std::string paramValue = endpoint[paramValueEndpointIndex];

            params.insert({paramIdentifier, paramValue});
        }
    }

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
}