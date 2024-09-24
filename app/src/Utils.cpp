#include "Utils.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

std::vector<std::string> Utils::split(std::string source, std::string delimiter, bool onlyFirstOccurrence) {    
    std::vector<std::string> splittedStr;

    size_t delimiterPos = source.find(delimiter);

    if(onlyFirstOccurrence) {
        if(delimiterPos != std::string::npos){
            std::string element = source.substr(0, delimiterPos);

            splittedStr.push_back(element);

            source = source.substr(delimiterPos+delimiter.size());

            delimiterPos = source.find(delimiter);
        }
        splittedStr.push_back(source);
    } else {
        while(delimiterPos != std::string::npos){
            std::string element = source.substr(0, delimiterPos);

            splittedStr.push_back(element);

            source = source.substr(delimiterPos+delimiter.size());

            delimiterPos = source.find(delimiter);
        }
        splittedStr.push_back(source);
    }
    
    if(splittedStr.size() > 0 && splittedStr.front() == "") splittedStr.erase(splittedStr.begin());
    if(splittedStr.size() > 0 && splittedStr.back() == "") splittedStr.erase(splittedStr.end() - 1);

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