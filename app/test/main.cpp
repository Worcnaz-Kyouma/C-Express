#include <iostream>
#include "Socket.hpp"
#include "thread"

int main() {
    Socket* myServerSocket = Socket::getSocket();

    myServerSocket->bindSocket(3001);
    myServerSocket->listenSocket(5);

    //while(true) {
        Socket* myClientSocket = myServerSocket->acceptSocket();

        const char* request = myClientSocket->readSocket();

        std::cout << request << std::endl;

        char* response = "HTTP/1.1 200 OK\r\nX-Powered-By: Express\r\nContent-Type: application/json; charset=utf-8\r\nContent-Length: 18\r\nETag: W/\"12-jXYefKeWu2RRHkrOsIrhrknCMXw\"\r\nDate: Thu, 08 Aug 2024 20:39:30 GMT\r\nConnection: close\r\n\n{\"success\":\"true\"}";

        myClientSocket->writeSocket(response, 255);
    //}
    
    delete myClientSocket;
    delete myServerSocket;

    return 0;
}