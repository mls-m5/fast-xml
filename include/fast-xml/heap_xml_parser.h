#pragma once

#include "heap_xml_tokenizer.h"
#include "heapxmlnode.h"
#include "heapxmltoken.h"
#include <iostream>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>

namespace fastxml {

/// Function used only for comparison, parse instead
HeapXmlNode parse_on_heap(std::vector<HeapXmlToken>::const_iterator &it,
                          std::vector<HeapXmlToken>::const_iterator end) {

    if (it->type() == TokenType::TEXT_CONTENT) {
        HeapXmlNode node(HeapXmlNode::Type::TEXT_CONTENT, "");
        node.content(it->str());
        ++it;
        return node;
    }

    if (it->type() != TokenType::ELEMENT_OPEN) {
        throw std::runtime_error{"expected opening tag"};
    }

    HeapXmlNode node(HeapXmlNode::Type::ELEMENT, "");

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
                node.add_child(parse_on_heap(it, end));
            }
            return node;
        }
        else if (token.type() == TokenType::ELEMENT_CLOSE) {
            ++it;
            return node;
        }
        else if (token.type() == TokenType::TEXT_CONTENT) {
            node.add_child(parse_on_heap(it, end));
        }
        else {
            throw std::runtime_error{"Invalid token"};
        }
    }

    throw std::runtime_error("Invalid XML format: Missing element closing tag");
}

HeapXmlNode parseSlow(std::istream &input) {
    std::vector<HeapXmlToken> tokens = tokenize_on_heap(input);
    auto it = tokens.cbegin();

    return parse_on_heap(it, tokens.cend());
}

} // namespace fastxml
