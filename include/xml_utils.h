#pragma once

#include <string>
#include <vector>

#include "xmltoken.h"
std::string strip(const std::string &str) {
    const auto start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) {
        return "";
    }
    const auto end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}
