#include "Utils.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

// Changed... now will have empty string on begin and end
std::vector<std::string> Utils::split(std::string source, std::string delimiter, bool onlyFirstOccurrence) {    
    std::vector<std::string> splittedStr;

    auto elementStart = std::find(source.begin(), source.end(), delimiter);

    while(elementStart != source.end()){
        std::string element(source.begin(), elementStart == source.begin() 
            ?   elementStart
            :   elementStart - 1
        );

        splittedStr.push_back(element);

        source = std::string(elementStart+delimiter.size(), source.end());

        elementStart = std::find(source.begin(), source.end(), delimiter);
        if(onlyFirstOccurrence) break;
    }
    splittedStr.push_back(source);

    return splittedStr;
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

std::string Utils::toLower(const std::string& source) {
    std::string destination;
    std::transform(source.begin(), source.end(), std::back_inserter(destination), [](char c) {
        return std::tolower(c);
    });

    return destination;
}

std::string Utils::toUpper(const std::string& source) {
    std::string destination;
    std::transform(source.begin(), source.end(), std::back_inserter(destination), [](char c) {
        return std::toupper(c);
    });

    return destination;
}