#include <iostream>
#include <cstring>
#include <chrono>
#include "Socket.hpp"
#include "Server.hpp"

void socketTest();
void testServerEndPointCreation();
void testServerEndPointRequest(); //Still remain the parseRequest and prepareRequest implementation

int main() {
    //socketTest();
    //testServerEndPointCreation();
    testServerEndPointRequest();

    return 0;
}

void testServerEndPointRequest() {
    
}

void testServerEndPointCreation() {
    Server myServer;

    // myServer.get("", [](Request req, Response res) {
    //     std::cout << "Root requested EPK!!!" << std::endl;
    // });

    // myServer.get("/", [](Request req, Response res) {
    //     std::cout << "Root requested EPK!!!" << std::endl;
    // });

    // myServer.get("planets", [](Request req, Response res) {
    //     std::cout << "Root requested EPK!!!" << std::endl;
    // });

    myServer.get("/planets", [](Request req, Response res) {
        std::cout << "Normal endpoint!!!" << std::endl;
    });

    // myServer.get("/planets/", [](Request req, Response res) {
    //     std::cout << "Normal weird endpoint... bruh but are the same from above" << std::endl;
    // });

    // myServer.get("/planets//", [](Request req, Response res) {
    //     std::cout << "The last slash is always ignorated if stands alone, like there" << std::endl;
    // });

    myServer.get("/planets/:id", [](Request req, Response res) {
        std::cout << "End point with fcking generic URL fragment!!!" << std::endl;
    });

    myServer.get("/planets/:id/asteroids", [](Request req, Response res) {
        std::cout << "End point with fcking generic URL fragment!!!" << std::endl;
    });
    
    myServer.promptResources();
}

void socketTest() {
    Socket* myServerSocket = Socket::getSocket();

    myServerSocket->bindSocket(3000);
    myServerSocket->listenSocket(5);

    Socket* myClientSocket = myServerSocket->acceptSocket();

    const char* request = myClientSocket->readSocket();

    std::cout << request << std::endl;

    char* response = "HTTP/1.1 200 OK\r\nContent-Type: application/json; charset=utf-8\r\nContent-Length: 18\r\nConnection: close\r\n\n{\"success\":\"true\"}";
    size_t responseSize = strlen(response);

    myClientSocket->writeSocket(response, responseSize);
    
    delete myClientSocket;
    delete myServerSocket;
}