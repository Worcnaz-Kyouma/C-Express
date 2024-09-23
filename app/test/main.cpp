#include <iostream>
#include <cstring>
#include <chrono>
#include "Utils.hpp"
#include "Server.hpp"

bool testServerCreation();
bool testUtils();
bool testServerEndpoints();

int main() {
    try{
        testServerEndpoints();
        return 0;
    } catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 0;
    }
}

// Successfull
bool testUtils() {
    std::vector<std::string> arr1 = Utils::split("d", '/');
    std::vector<std::string> arr2 = Utils::split("/", '/');
    std::vector<std::string> arr3 = Utils::split("a/", '/');
    std::vector<std::string> arr4 = Utils::split("/a", '/');
    std::vector<std::string> arr5 = Utils::split("a/b", '/');
    auto arrs = {
        arr1,
        arr2,
        arr3,
        arr4,
        arr5
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

    std::cout << Utils::join({"te", "st"}, "/") << std::endl;

    std::cout << Utils::toLower("DaMn") << std::endl;
    std::cout << Utils::toUpper("dAmN") << std::endl;

    std::cout << Utils::hasDuplicate<std::string>({"here", "there", "everywhere"}) << std::endl;
    std::cout << Utils::hasDuplicate<std::string>({"here", "there", "everywhere", "here"}) << std::endl;
}

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

    server.get("/planets/:id_planet//", [](Request* req, Response* res) {
        std::cout << "NICEEE!!!!!!!!!" << std::endl;
    });

    server.get("", [](Request* req, Response* res) {
        std::cout << "NICEEE!!!!!!!!!" << std::endl;
    });

    server.promptEndpoints();
    return true;
}

bool testServerCreation() {
    Server server;

    server.listen(3435);
    return true;
}