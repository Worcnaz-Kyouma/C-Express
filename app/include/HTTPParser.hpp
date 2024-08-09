#ifndef CE_HTTPPARSER_H
#define CE_HTTPPARSER_H

#include <array>

class HTTPParser {
private:
    const static std::array<char*, 2> ENABLED_METHODS; // {'GET', 'POST'}
};

#endif