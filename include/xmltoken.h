#pragma once

#include <ostream>
#include <string>
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

std::string strip(const std::string &str) {
    const auto start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) {
        return "";
    }
    const auto end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

class XmlToken {
public:
    XmlToken(TokenType type = TokenType::UNKNOWN,
             std::string value = "",
             std::size_t line = 0,
             std::size_t col = 0)
        : _type(type)
        , _value(strip(value))
        , _line(line)
        , _col(col) {}

    std::string_view str() const {
        return _value;
    }

    std::size_t line() const {
        return _line;
    }

    std::size_t col() const {
        return _col;
    }

    TokenType type() const {
        return _type;
    }

    bool operator==(const XmlToken other) const {
        return type() == other.type() && str() == other.str();
    }

private:
    TokenType _type;
    std::string _value;
    std::size_t _line;
    std::size_t _col;
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

std::ostream &operator<<(std::ostream &os, const XmlToken &token) {
    os << "('" << token.str() << "', " << to_string(token.type()) << ", "
       << token.line() << ", " << token.col() << ")";
    return os;
}
