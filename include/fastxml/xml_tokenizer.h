#pragma once

#include "tokentype.h"
#include "xmlfile.h"
#include "xmltoken.h"
#include <cctype>
#include <istream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace fastxml {

inline std::vector<XmlToken> tokenize(XmlFile::Reader &input) {
    std::vector<XmlToken> tokens;
    std::string_view current_token;
    // std::size_t line = 1, col = 1;
    const char *ch = nullptr;

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

    auto line = [&] { return input.line(); };
    auto col = [&] { return input.col(); };
    auto next = [&] { input.get(ch); };

    if (std::isspace(input.peek())) {
        input.get(ch); // This is because next() requires ch to be set
    }

    auto removeSpaces = [&input, &next] {
        while (std::isspace(input.peek())) {
            next();
        }
    };

    removeSpaces();

    if (input.peek() == '<' && input.peek(1) == '?') {
        for (; input.get(ch);) {
            if (*ch == '?' && input.peek() == '>') {
                input.get(ch);

                removeSpaces();
                break;
            }
        }
    }

    auto addCh = [](std::string_view &sv, const char *ch) {
        if (sv.empty()) {
            sv = std::string_view{ch, 1};
            return;
        }
        sv = std::string_view{sv.data(), sv.size() + 1};
    };

    while (input.get(ch)) {
        switch (state) {
        case State::TEXT:
            // This expects all spaces to be removed beforehand
            if (*ch == '<') {
                // Remove comments
                if (input.peek() == '!' && input.peek(1) == '-' &&
                    input.peek(2) == '-') {

                    next();
                    next();
                    next();
                    next();
                    for (; input.get(ch);) {
                        if (*ch == '-' && input.peek() == '-' &&
                            input.peek(1) == '>') {
                            next();
                            next();
                            next();
                            removeSpaces();
                            break;
                        }
                    }

                    continue;
                }

                current_token = strip(std::move(current_token));
                if (!current_token.empty()) {
                    // TODO: Possible count the required amount of tokens in
                    // forehand to remove need for resizing this vector
                    // However it does not seem to be the biggest bottleneck.
                    tokens.emplace_back(TokenType::TEXT_CONTENT,
                                        current_token,
                                        input.line(),
                                        input.col() - current_token.size());
                    current_token = {};
                }
                state = State::OPEN_TAG;
            }
            else {
                addCh(current_token, ch);
            }
            break;
        case State::OPEN_TAG:
            if (*ch == '/') {
                state = State::CLOSE_TAG;
            }
            else if (std::isalnum(*ch)) {
                addCh(current_token, ch);
                state = State::TAG_NAME;
            }
            break;
        case State::TAG_NAME:
            if (*ch == ' ') {
                tokens.emplace_back(TokenType::ELEMENT_OPEN,
                                    current_token,
                                    line(),
                                    col() - current_token.size());
                current_token = {};
                state = State::ATTRIBUTE_NAME;
            }
            else if (*ch == '>') {
                tokens.emplace_back(TokenType::ELEMENT_OPEN,
                                    current_token,
                                    line(),
                                    col() - current_token.size());
                current_token = {};
                state = State::TEXT;

                if (current_token.empty()) {
                    while (input && std::isspace(input.peek())) {
                        next();
                        //                        input.get(ch);
                    }
                }
            }
            else if (*ch == '/') {
                tokens.emplace_back(TokenType::ELEMENT_OPEN,
                                    current_token,
                                    line(),
                                    col() - current_token.size());
                current_token = {};

                if (current_token.empty()) {
                    while (input && std::isspace(input.peek())) {
                        next();
                        //                        input.get(ch);
                    }
                }

                state = State::CLOSE_TAG;
                break;
            }
            else {
                addCh(current_token, ch);
            }
            break;
        case State::ATTRIBUTE_NAME:
            if (std::isspace(*ch)) {
                break;
            }
            if (*ch == '=') {
                tokens.emplace_back(TokenType::ATTRIBUTE_NAME,
                                    current_token,
                                    line(),
                                    col() - current_token.size());
                current_token = {};
                state = State::ATTRIBUTE_VALUE_START;
            }
            else if (*ch == '>') {
                if (!current_token.empty()) {
                    tokens.emplace_back(TokenType::ATTRIBUTE_NAME,
                                        current_token,
                                        line(),
                                        col() - current_token.size());
                    current_token = {};
                }
                state = State::TEXT;

                if (current_token.empty()) {
                    while (input && std::isspace(input.peek())) {
                        input.get(ch);
                    }
                }
            }
            else if (*ch == '/') {
                input.get(ch);
                if (*ch != '>') {
                    throw std::runtime_error{
                        std::to_string(line()) + ":" + std::to_string(col()) +
                        " expected '>' got " + std::string{ch, 1}};
                }
                tokens.emplace_back(TokenType::ELEMENT_CLOSE,
                                    "",
                                    line(),
                                    col() - current_token.size());
                state = State::TEXT;
            }
            else {
                addCh(current_token, ch);
            }
            break;
        case State::ATTRIBUTE_VALUE_START:
            if (*ch == '\"') {
                state = State::ATTRIBUTE_VALUE;
            }
            break;
        case State::ATTRIBUTE_VALUE:
            if (*ch == '\"') {
                tokens.emplace_back(TokenType::ATTRIBUTE_VALUE,
                                    current_token,
                                    line(),
                                    col() - current_token.size());
                current_token = {};
                state = State::ATTRIBUTE_NAME;
            }
            else {
                addCh(current_token, ch);
            }
            break;
        case State::CLOSE_TAG:
            if (*ch == '>') {
                state = State::TEXT;
                tokens.emplace_back(
                    TokenType::ELEMENT_CLOSE, current_token, line(), col());
                current_token = {};
            }
            else {
                addCh(current_token, ch);
            }
            break;
        case State::COMMENT:
            if (*ch == '-') {
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

        // if (*ch == '\n') {
        //     line++;
        //     col = 1;
        // }
        // else {
        //     col++;
        // }
    }

    if (!current_token.empty()) {
        tokens.emplace_back(
            TokenType::TEXT_CONTENT, current_token, line(), col());
        if (tokens.back().str().empty()) {
            tokens.pop_back();
        }
    }

    return tokens;
}

} // namespace fastxml
