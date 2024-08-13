#ifndef CE_UTILS_H
#define CE_UTILS_H

#include <vector>
#include <string>

class Utils {
public:
    static std::vector<std::string> split(const std::string& source, char delimiter);
};

#endif