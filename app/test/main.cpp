#include <iostream>
#include <cstring>
#include <chrono>
#include "Server.hpp"

bool testServerCreation();
bool testServerProcessCreation();
bool testServerResourceManager();

int main() {
    testServerCreation();
    std::cout << "NICEEE!!!!!!!!!" << std::endl;
    return 0;
}

bool testServerCreation() {
    Server server;

    server.get("/", [](Request req, Response res) {
        std::cout << "NICEEE!!!!!!!!!" << std::endl;
    });

    server.listen(3434);
    return true;
}