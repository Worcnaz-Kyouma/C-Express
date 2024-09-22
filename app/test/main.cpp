#include <iostream>
#include <cstring>
#include <chrono>
#include "socket//Socket.hpp"
#include "Server.hpp"

bool testServerCreation();
bool testServerProcessCreation();
bool testServerResourceManager();

int main() {
    testServerCreation();

    return 0;
}

bool testServerCreation() {
    Server myServer;

    myServer.get("/", [](Request req, Response res) {
        std::cout << "test!" << std::endl;
    });
}