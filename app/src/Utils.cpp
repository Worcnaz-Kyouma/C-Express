#include "Utils.hpp"
#include <iostream>
#include <sstream>

std::vector<std::string> Utils::split(const std::string& source, char delimiter) {
    std::vector<std::string> newArray;
    std::stringstream sStream(source);
    std::string element;

    while (std::getline(sStream, element, delimiter)) {
        newArray.push_back(element);
    }

    return newArray;
}