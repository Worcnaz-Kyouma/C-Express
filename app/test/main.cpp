#include <iostream>
#include "Socket.hpp"

int main() {
    Socket* myServerSocket = Socket::getSocket();

    myServerSocket->bindSocket(3000);
    myServerSocket->listenSocket(5);
    Socket* myClientSocket = myServerSocket->acceptSocket();

    const char* request = myClientSocket->readSocket();

    std::cout << request << std::endl;

    myClientSocket->writeSocket("HTTP/1.1 200 OK\nDate: Wed, 07 Aug 2024 12:00:00 GMT\nServer: Apache/2.4.41 (Ubuntu)\nContent-Type: application/json; charset=UTF-8\nConnection: close \n\n{\nsuccess: true\n}", 124);

    delete myClientSocket;
    delete myServerSocket;

    return 0;
}