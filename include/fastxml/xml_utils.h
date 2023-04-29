#pragma once

#include <string>
#include <vector>

namespace fastxml {

inline std::string_view strip(std::string_view s) {
    // Find the index of the first non-whitespace character
    auto first_non_ws = s.find_first_not_of(" \t\n\r");

    // If the string is empty or consists only of whitespace characters, return
    // an empty string
    if (first_non_ws == std::string_view::npos) {
        return "";
    }

    // Find the index of the last non-whitespace character
    auto last_non_ws = s.find_last_not_of(" \t\n\r");

    // Return a string view of the non-whitespace portion of the input string
    return s.substr(first_non_ws, last_non_ws - first_non_ws + 1);
}

} // namespace fastxml
