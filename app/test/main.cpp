#include <iostream>
#include <cstring>
#include <chrono>
#include "Server.hpp"

bool testServerCreation();

int main() {
    try{
        testServerCreation();
        return 0;
    } catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
}

bool testServerCreation() {
    Server server;

    server.get("/", [](Request* req, Response* res) {
        std::cout << "NICEEE!!!!!!!!!" << std::endl;
    });

    server.listen(3435);
    return true;
}