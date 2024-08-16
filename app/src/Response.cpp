#include <string>
#include <vector>
#include <iostream>
#include "Response.hpp"

Response::Response(Socket* clientSocket): clientSocket(clientSocket) {};

void Response::send(const std::string& message) {
    const auto messageSize = message.size();

    const std::string responseMsg = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: " + std::to_string(messageSize) + "\nConnection: close\n\n" + message;
    std::cout << responseMsg << std::endl;
    std::cout << responseMsg.size() << std::endl;
    std::vector<char> v(responseMsg.begin(), responseMsg.end());
    char* responseMsg_c = v.data();

    this->clientSocket->writeSocket(responseMsg_c, responseMsg.size());

}