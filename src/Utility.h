#pragma once

#include <string>
#include <vector>
#include <windows.h>

#include "PCH.h"

namespace Utility {
    std::string GetCurrentExecutableDirectory();
    std::vector<std::string> Split(const std::string &str, char delim);
}