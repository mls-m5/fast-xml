#pragma once

#include "tokentype.h"
#include "xml_utils.h"
#include <ostream>
#include <string>
#include <string_view>

namespace fastxml {

class HeapXmlToken {
public:
    HeapXmlToken(TokenType type = TokenType::UNKNOWN,
                 std::string value = "",
                 std::size_t line = 0,
                 std::size_t col = 0);

    std::string_view str() const;

    std::size_t line() const;

    std::size_t col() const;

    TokenType type() const;

    bool operator==(const HeapXmlToken other) const;

private:
    TokenType _type;
    std::string _value;
    std::size_t _line;
    std::size_t _col;
};

std::string join_tokens(const std::vector<HeapXmlToken> &tokens) {
    std::string result;
    for (const auto &token : tokens) {
        if (token.type() == TokenType::TEXT_CONTENT) {
            result.append(token.str());
        }
    }
    return result;
}

std::ostream &operator<<(std::ostream &os, const HeapXmlToken &token) {
    os << "('" << token.str() << "', " << to_string(token.type()) << ", "
       << token.line() << ", " << token.col() << ")";
    return os;
}

inline HeapXmlToken::HeapXmlToken(TokenType type,
                                  std::string value,
                                  std::size_t line,
                                  std::size_t col)
    : _type(type)
    , _value(strip(value))
    , _line(line)
    , _col(col) {}

inline std::string_view HeapXmlToken::str() const {
    return _value;
}

inline std::size_t HeapXmlToken::line() const {
    return _line;
}

inline std::size_t HeapXmlToken::col() const {
    return _col;
}

inline TokenType HeapXmlToken::type() const {
    return _type;
}

inline bool HeapXmlToken::operator==(const HeapXmlToken other) const {
    return type() == other.type() && str() == other.str();
}

} // namespace fastxml
