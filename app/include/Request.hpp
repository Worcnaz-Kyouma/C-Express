#ifndef CE_REQUEST_H
#define CE_REQUEST_H

class Request {
public:
    const std::string method;
    const std::string url;

    Request& operator=(const Request $other);
};

#endif