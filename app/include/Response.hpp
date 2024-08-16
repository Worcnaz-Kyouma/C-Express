#ifndef CE_RESPONSE_H
#define CE_RESPONSE_H

#include "Socket.hpp"

class Response {
private:
    Socket* clientSocket;
public:
    Response(Socket* clientSocket);

    void send(const std::string& message);
};

#endif