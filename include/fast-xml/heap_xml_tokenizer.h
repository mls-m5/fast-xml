// xml_parser.h
#include "heapxmltoken.h"
#include <cctype>
#include <istream>
#include <vector>

/// Tokenizer used for comparison, use tokenize instead
std::vector<HeapXmlToken> tokenize_on_heap(std::istream &input) {
    std::vector<HeapXmlToken> tokens;
    std::string current_token;
    std::size_t line = 1, col = 1;
    char ch;

    enum class State {
        TEXT,
        OPEN_TAG,
        TAG_NAME,
        ATTRIBUTE_NAME,
        ATTRIBUTE_VALUE_START,
        ATTRIBUTE_VALUE,
        CLOSE_TAG,
        COMMENT,
        CDATA
    };

    State state = State::TEXT;

    while (std::isspace(input.peek())) {
        input.get(ch);
    }

    while (input.get(ch)) {
        switch (state) {
        case State::TEXT:
            if (ch == '<') {
                current_token = strip(std::move(current_token));
                if (!current_token.empty()) {
                    tokens.emplace_back(TokenType::TEXT_CONTENT,
                                        current_token,
                                        line,
                                        col - current_token.size());
                    current_token.clear();
                }
                state = State::OPEN_TAG;
            }
            else {
                current_token += ch;
            }
            break;
        case State::OPEN_TAG:
            if (ch == '/') {
                state = State::CLOSE_TAG;
            }
            else if (std::isalnum(ch)) {
                current_token += ch;
                state = State::TAG_NAME;
            }
            break;
        case State::TAG_NAME:
            if (ch == ' ') {
                tokens.emplace_back(TokenType::ELEMENT_OPEN,
                                    current_token,
                                    line,
                                    col - current_token.size());
                current_token.clear();
                state = State::ATTRIBUTE_NAME;
            }
            else if (ch == '>') {
                tokens.emplace_back(TokenType::ELEMENT_OPEN,
                                    current_token,
                                    line,
                                    col - current_token.size());
                current_token.clear();
                state = State::TEXT;

                if (current_token.empty()) {
                    while (input && std::isspace(input.peek())) {
                        input.get(ch);
                    }
                }
            }
            else if (ch == '/') {
                tokens.emplace_back(TokenType::ELEMENT_OPEN,
                                    current_token,
                                    line,
                                    col - current_token.size());
                current_token = {};

                if (current_token.empty()) {
                    while (input && std::isspace(input.peek())) {
                        input.get(ch);
                    }
                }

                state = State::CLOSE_TAG;
                break;
            }
            else {
                current_token += ch;
            }
            break;
        case State::ATTRIBUTE_NAME:
            if (ch == '=') {
                tokens.emplace_back(TokenType::ATTRIBUTE_NAME,
                                    current_token,
                                    line,
                                    col - current_token.size());
                current_token.clear();
                state = State::ATTRIBUTE_VALUE_START;
            }
            else if (ch == '>') {
                if (!current_token.empty()) {
                    tokens.emplace_back(TokenType::ATTRIBUTE_NAME,
                                        current_token,
                                        line,
                                        col - current_token.size());
                    current_token.clear();
                }
                state = State::TEXT;

                if (current_token.empty()) {
                    while (input && std::isspace(input.peek())) {
                        input.get(ch);
                    }
                }
            }
            else if (ch == '/') {
                input.get(ch);
                if (ch != '>') {
                    throw std::runtime_error{"expected '>' got " +
                                             std::string{ch, 1}};
                }
                tokens.emplace_back(TokenType::ELEMENT_CLOSE,
                                    "",
                                    line,
                                    col - current_token.size());
                state = State::TEXT;
            }
            else {
                current_token += ch;
            }
            break;
        case State::ATTRIBUTE_VALUE_START:
            if (ch == '\"') {
                state = State::ATTRIBUTE_VALUE;
            }
            break;
        case State::ATTRIBUTE_VALUE:
            if (ch == '\"') {
                tokens.emplace_back(TokenType::ATTRIBUTE_VALUE,
                                    current_token,
                                    line,
                                    col - current_token.size());
                current_token.clear();
                state = State::ATTRIBUTE_NAME;
            }
            else {
                current_token += ch;
            }
            break;
        case State::CLOSE_TAG:
            if (ch == '>') {
                state = State::TEXT;
                tokens.emplace_back(
                    TokenType::ELEMENT_CLOSE, current_token, line, col);
                current_token.clear();
            }
            else {
                current_token += ch;
            }
            break;
        case State::COMMENT:
            if (ch == '-') {
                // Ignore comments for now
            }
            else {
                state = State::TEXT;
            }
            break;
        case State::CDATA:
            // Ignore CDATA for now
            break;
        }

        if (ch == '\n') {
            line++;
            col = 1;
        }
        else {
            col++;
        }
    }

    if (!current_token.empty()) {
        tokens.emplace_back(TokenType::TEXT_CONTENT, current_token, line, col);
        if (tokens.back().str().empty()) {
            tokens.pop_back();
        }
    }

    return tokens;
}
