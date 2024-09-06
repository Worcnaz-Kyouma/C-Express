#include "Utils.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>

std::vector<std::string> Utils::split(const std::string& source, char delimiter, bool onlyFirstOccurrence = false) {
    std::vector<std::string> newArray;
    std::stringstream sStream(source);
    std::string element;

    if(onlyFirstOccurrence) {
        std::string remainder;

        std::getline(sStream, element, delimiter);
        std::getline(sStream, remainder);
        
        newArray.push_back(element);
        newArray.push_back(remainder);
    } else {
        while (std::getline(sStream, element, delimiter)) {
            newArray.push_back(element);
        }
        if(newArray[0].empty()) {
            newArray.erase(newArray.begin());
        }
    }

    return newArray;
}

template <typename T>
bool Utils::hasDuplicate(std::vector<T> source) {
    for(auto element = source.begin(); element != source.end(); element++) {
        source.erase(element);

        bool haveFoundAnother = std::find(source.begin(), source.end(), *element);
        if(haveFoundAnother) return true;
    }
    return false;
}