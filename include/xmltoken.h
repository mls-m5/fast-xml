#pragma once

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
             std::size_t col = 0)
        : _type(type)
        , _value(value)
        , _line(line)
        , _col(col) {}

    std::string_view str() const {
        switch (_type) {
        case TokenType::ATTRIBUTE_NAME:
        case TokenType::ATTRIBUTE_VALUE:
            return std::string_view(_value);
        default:
            return std::string_view();
        }
    }

    std::size_t line() const {
        return _line;
    }

    std::size_t col() const {
        return _col;
    }

private:
    TokenType _type;
    std::string _value;
    std::size_t _line;
    std::size_t _col;
};
