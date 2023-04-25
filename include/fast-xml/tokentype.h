#pragma once

#include <string_view>

enum class TokenType {
    UNKNOWN,         // Unknown token type
    ELEMENT_OPEN,    // <element>
    ELEMENT_CLOSE,   // </element>
    ATTRIBUTE_NAME,  // attr
    ATTRIBUTE_VALUE, // "value"
    TEXT_CONTENT,    // text between elements
    COMMENT,         // <!-- comment -->
    CDATA            // <![CDATA[data]]>
};

std::string_view to_string(TokenType type) {
    switch (type) {
    case TokenType::UNKNOWN:
        return "UNKNOWN";
    case TokenType::ELEMENT_OPEN:
        return "ELEMENT_OPEN";
    case TokenType::ELEMENT_CLOSE:
        return "ELEMENT_CLOSE";
    case TokenType::ATTRIBUTE_NAME:
        return "ATTRIBUTE_NAME";
    case TokenType::ATTRIBUTE_VALUE:
        return "ATTRIBUTE_VALUE";
    case TokenType::TEXT_CONTENT:
        return "TEXT_CONTENT";
    case TokenType::COMMENT:
        return "COMMENT";
    case TokenType::CDATA:
        return "CDATA";
    default:
        return "UNKNOWN";
    }
}

inline std::ostream &operator<<(std::ostream &os, TokenType type) {
    os << to_string(type);
    return os;
}
