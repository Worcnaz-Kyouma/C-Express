#include <iostream>
#include <cstring>
#include <chrono>
#include "Utils.hpp"
#include "Server.hpp"

bool testServerCreation();
bool testUtils();
bool testServerEndpoints();
bool testServerParamsAndQuery();

int main() {
    try{
        testServerParamsAndQuery();
        return 0;
    } catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
}

// Successfull
bool testUtils() {
    std::vector<std::string> arr4 = Utils::split("", "/");
    std::vector<std::string> arr5 = Utils::split("a/b/", "/", true);
    std::vector<std::string> arr6 = Utils::split("a/b//", "/");
    auto arrs = {
        arr4,
        arr5,
        arr6
    };

    int count = 0;
    std::for_each(arrs.begin(), arrs.end(), [&count](auto arr) {
        count++;

        std::cout << count << std::endl;
        std::for_each(arr.begin(), arr.end(), [](auto frag) {
            std::cout << "Element: " << frag << std::endl;
        });

        std::cout << std::endl;
    });
}

// Successfull
bool testServerEndpoints() {
    Server server;
    
    server.get("/", [](Request* req, Response* res) {
        std::cout << "NICEEE!!!!!!!!!" << std::endl;
    });

    server.get("/planets", [](Request* req, Response* res) {
        std::cout << "NICEEE!!!!!!!!!" << std::endl;
    });

    server.get("/planets/:id_planet", [](Request* req, Response* res) {
        std::cout << "NICEEE!!!!!!!!!" << std::endl;
    });

    server.get("/planets/:id_planet/", [](Request* req, Response* res) {
        std::cout << "NICEEE!!!!!!!!!" << std::endl;
    });

    server.get("/planets/:id_planet/:id_planet", [](Request* req, Response* res) {
        std::cout << "NICEEE!!!!!!!!!" << std::endl;
    });

    // server.get("", [](Request* req, Response* res) {
    //     std::cout << "NICEEE!!!!!!!!!" << std::endl;
    // });

    server.promptEndpoints();
    return true;
}

// Seems like express js is more fast than C-Express... ;~; But success!
bool testServerCreation() {
    Server server;

    server.get("/", [](Request* req, Response* res) {
        std::cout << "Hell yeah, welcome to world C-Express" << std::endl;
        res->send(std::string("HERRROOOO! From C-Express my sir!"));
    });
    
    server.get("/json", [](Request* req, Response* res) {
        std::cout << "First things first, beginning with json" << std::endl;
        json myJson = {
            { "server", "Damn, its C-Express" },
            { "creator", "Worcnaz Prado, a.k.a Nicolas Almeida Prado" }
        };

        res->send(myJson);
    });

    server.listen(3435);
    return true;
}

bool testServerParamsAndQuery() {
    Server server;

    server.get("/planet/:id_planet", [](Request* req, Response* res) {
        std::string planetId = req->params.at("id_planet");
        std::cout << "The planet id selected is: " << planetId << std::endl;

        std::string planetName = req->query.at("planet_name");
        std::cout << "The planet name selected is: " << planetName << std::endl;

        res->send("The planet selected is: " + planetId+"\nThe name is: " + planetName);
    });

    server.listen(3434);
    return true;
}