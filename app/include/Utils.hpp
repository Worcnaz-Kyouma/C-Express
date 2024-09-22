#ifndef CE_UTILS_H
#define CE_UTILS_H

#include <vector>
#include <string>
#include <algorithm>

class Utils {
public:
    static std::vector<std::string> split(std::string source, char delimiter, bool onlyFirstOccurrence = false, bool ignoreEscapedOccurrences = false);
    static std::string join(std::vector<std::string> source, std::string separator);
    template <typename T>
    static bool hasDuplicate(std::vector<T> source);
    static std::string toLower(const std::string& source);
    static std::string toUpper(const std::string& source);
};

// Template function implementation
template <typename T>
bool Utils::hasDuplicate(std::vector<T> source) {
    for(auto element = source.begin(); element != source.end(); element++) {
        source.erase(element);

        bool haveFoundAnother = (std::find(source.begin(), source.end(), *element) != source.end());
        if(haveFoundAnother) return true;
    }
    return false;
}

#endif