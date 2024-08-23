#include <iostream>
#include <cstring>
#include <chrono>
#include "Socket.hpp"
#include "Server.hpp"

// PRE-ALPHA
void socketTest_PreAlpha();
void testServerConnection_PreAlpha();
void testServerEndPointCreation_PreAlpha();
void testServerEndPointRequest_PreAlpha();

int main() {
    return 0;
}

void testServerEndPointRequest_PreAlpha() {
    Server myServer;

    myServer.get("/", [](Request req, Response res) {
        res.send("Root!");
    });

    myServer.get("/epk", [](Request req, Response res) {
        res.send("EPK!!!");
    });

    myServer.get("/epk/:labman", [](Request req, Response res) {
        res.send("Hououin Kyouma");
    });

    myServer.listen(3000);
}

void testServerConnection_PreAlpha() {
    Server myServer;

    myServer.get("/", [](Request req, Response res) {
        res.send("Worked!");
    });

    myServer.listen(3000);
}

void testServerEndPointCreation_PreAlpha() {
    Server myServer;

    myServer.get("/", [](Request req, Response res) {
        std::cout << "Root requested EPK!!!" << std::endl;
    });

    myServer.get("/planets", [](Request req, Response res) {
        std::cout << "Normal endpoint!!!" << std::endl;
    });


    myServer.get("/planets/:id", [](Request req, Response res) {
        std::cout << "End point with fcking generic URL fragment!!!" << std::endl;
    });

    myServer.get("/planets/:id/asteroids", [](Request req, Response res) {
        std::cout << "End point with fcking generic URL fragment!!!" << std::endl;
    });
    
    myServer.promptResources();
}

void socketTest_PreAlpha() {
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