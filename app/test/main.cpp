#include <iostream>
#include <cstring>
#include <chrono>
#include "Socket.hpp"

int main() {
    Socket* myServerSocket = Socket::getSocket();

    myServerSocket->bindSocket(3000);
    myServerSocket->listenSocket(5);

    std::thread myThread([]() { 
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "test" << std::endl; 
    });
    Socket* myClientSocket = myServerSocket->acceptSocket();

    const char* request = myClientSocket->readSocket();

    std::cout << request << std::endl;

    char* response = "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\nContent-Length: 18\r\nConnection: close\r\n\n{\"success\":\"true\"}";
    size_t responseSize = strlen(response);

    myClientSocket->writeSocket(response, responseSize);
    
    delete myClientSocket;
    delete myServerSocket;

    return 0;
}