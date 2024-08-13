#ifndef CE_HTTPPARSER_H
#define CE_HTTPPARSER_H

#include <array>
#include <string>

class HTTPParser {
private:
    const static std::array<char*, 2> ENABLED_METHODS; // {'GET', 'POST'}
public: 
    Request parseRequest(const std::string& rawRequest) const;
};

#endif