#pragma once

#include "slow_xml_tokenizer.h"
#include "slowxmlnode.h"
#include "slowxmltoken.h"
#include <iostream>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>

SlowXmlNode parseSlow(std::vector<SlowXmlToken>::const_iterator &it,
                      std::vector<SlowXmlToken>::const_iterator end) {

    if (it->type() == TokenType::TEXT_CONTENT) {
        SlowXmlNode node(SlowXmlNode::Type::TEXT_CONTENT, "");
        node.content(it->str());
        ++it;
        return node;
    }

    if (it->type() != TokenType::ELEMENT_OPEN) {
        throw std::runtime_error{"expected opening tag"};
    }

    SlowXmlNode node(SlowXmlNode::Type::ELEMENT, "");

    node.name(it->str());
    ++it;

    // Parse the attributes
    while (it != end && it->type() == TokenType::ATTRIBUTE_NAME) {
        auto attribute_name = it->str();
        if ((it + 1) == end || (it + 1)->type() != TokenType::ATTRIBUTE_VALUE) {
            throw std::runtime_error(
                "Invalid XML format: Missing attribute value");
        }
        ++it;
        auto attribute_value = it->str();
        node.attributes().push_back({attribute_name, attribute_value});
        ++it;
    }

    while (it != end) {
        const auto &token = *it;
        if (token.type() == TokenType::ELEMENT_OPEN) {
            // Recursively parse the children
            while (it != end && it->type() != TokenType::ELEMENT_CLOSE) {
                node.add_child(parseSlow(it, end));
            }
            return node;
        }
        else if (token.type() == TokenType::ELEMENT_CLOSE) {
            ++it;
            return node;
        }
        else if (token.type() == TokenType::TEXT_CONTENT) {
            node.add_child(parseSlow(it, end));
        }
        else {
            throw std::runtime_error{"Invalid token"};
        }
    }

    throw std::runtime_error("Invalid XML format: Missing element closing tag");
}

SlowXmlNode parseSlow(std::istream &input) {
    std::vector<SlowXmlToken> tokens = tokenizeSlow(input);
    auto it = tokens.cbegin();

    return parseSlow(it, tokens.cend());
}
