#pragma once

#include "xml_utils.h"
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

class XmlToken {
public:
    XmlToken(TokenType type = TokenType::UNKNOWN,
             std::string value = "",
             std::size_t line = 0,
             std::size_t col = 0);

    std::string_view str() const;

    std::size_t line() const;

    std::size_t col() const;

    TokenType type() const;

    bool operator==(const XmlToken other) const;

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

std::string join_tokens(const std::vector<XmlToken> &tokens) {
    std::string result;
    for (const auto &token : tokens) {
        if (token.type() == TokenType::TEXT_CONTENT) {
            result.append(token.str());
        }
    }
    return result;
}

inline XmlToken::XmlToken(TokenType type,
                          std::string value,
                          std::size_t line,
                          std::size_t col)
    : _type(type)
    , _value(strip(value))
    , _line(line)
    , _col(col) {}

inline std::string_view XmlToken::str() const {
    return _value;
}

inline std::size_t XmlToken::line() const {
    return _line;
}

inline std::size_t XmlToken::col() const {
    return _col;
}

inline TokenType XmlToken::type() const {
    return _type;
}

inline bool XmlToken::operator==(const XmlToken other) const {
    return type() == other.type() && str() == other.str();
}
