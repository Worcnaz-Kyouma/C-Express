#include "Utils.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

std::vector<std::string> Utils::split(std::string source, char delimiter, bool onlyFirstOccurrence, bool ignoreInnerString) {    
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
        if(ignoreInnerString) {
            bool isInsideString = false;
            while (std::getline(sStream, element, delimiter)) {
                bool foundEscapeChar = false;
                std::for_each(element.begin(), element.end(), [&isInsideString, &foundEscapeChar](char c) {
                    if(!foundEscapeChar && c == '"'){
                        isInsideString = !isInsideString;
                    }
                    if(c == '\\') {
                        foundEscapeChar = true;
                    } else {
                        foundEscapeChar = false;
                    }
                });
                if(!isInsideString) newArray.push_back(element);
            }
            
        }
        while (std::getline(sStream, element, delimiter)) {
            newArray.push_back(element);
        }
        if(newArray[0].empty()) {
            newArray.erase(newArray.begin());
        }
    }

    return newArray;
}

std::string Utils::join(std::vector<std::string> source, std::string separator) {
    std::ostringstream result;

    result << *source.begin();
    for(auto sourceElement = source.begin()+1; sourceElement != source.end(); sourceElement++) {
        result << separator;
        result << *sourceElement;
    }

    return result.str();
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

std::string Utils::toLower(const std::string& source) {
    std::string destination;
    std::transform(source.begin(), source.end(), destination.begin(), [](char c) {
        return std::tolower(c);
    });

    return destination;
}

std::string Utils::toUpper(const std::string& source) {
    std::string destination;
    std::transform(source.begin(), source.end(), destination.begin(), [](char c) {
        return std::toupper(c);
    });

    return destination;
}