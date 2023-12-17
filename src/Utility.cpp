#pragma once
#include "Utility.h"

namespace Utility {

    std::string GetCurrentExecutableDirectory() {
        char buffer[MAX_PATH];
        GetModuleFileNameA(nullptr, buffer, MAX_PATH);
        std::string::size_type pos = std::string(buffer).find_last_of("\\/");
        return std::string(buffer).substr(0, pos);
    }

    std::vector<std::string> Split(const std::string &str, char delim) {
        std::vector<std::string> res;
        size_t current = 0, found;
        while ((found = str.find_first_of(delim, current)) != std::string::npos) {
            res.push_back(std::string(str, current, found - current));
            current = found + 1;
        }
        res.push_back(std::string(str, current, str.size() - current));
        return res;
    }
}