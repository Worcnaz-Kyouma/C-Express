#ifndef CE_HTTPPARSER1X0_H
#define CE_HTTPPARSER1X0_H

#include "HTTPParser.hpp"

class HTTPParser1x0 : public HTTPParser {
public:
    Request* generateRequest() override;
    Response* generateResponse() override;
};

#endif