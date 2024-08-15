#include <string>
#include "Response.hpp"

Response::Response(Socket* clientSocket): clientSocket(clientSocket) {};

void Response::send(const std::string& message) {
    
}