#pragma once

#include "xml_tokenizer.h"
#include "xmlnode.h"
#include <iostream>
#include <stack>

XmlNode parse(std::istream &input) {
    std::vector<XmlToken> tokens = tokenize(input);
    auto it = tokens.begin();
    std::stack<std::string_view> tag_stack;

    std::cout << "Tokens:\n";
    for (const auto &token : tokens) {
        std::cout << token << std::endl;
    }

    XmlNode root(XmlNode::Type::ELEMENT, "root");

    while (it != tokens.end()) {
        const auto &token = *it;

        if (token.type() == TokenType::ELEMENT_OPEN) {
            tag_stack.push(token.str());

            XmlNode node(XmlNode::Type::ELEMENT, token.str());
            if (!root.name().empty()) {
                root.add_child(node);
            }
            else {
                root = std::move(node);
            }
        }
        else if (token.type() == TokenType::ELEMENT_CLOSE) {
            if (tag_stack.empty()) {
                // Invalid XML format: Closing tag without a matching opening
                // tag
                throw std::runtime_error("Invalid XML format");
            }

            std::string_view expected_tag = tag_stack.top();
            tag_stack.pop();

            // Check that the closing tag matches the opening tag
            if (expected_tag != token.str()) {
                // Invalid XML format: Mismatched opening and closing tags
                std::cerr << "Error: expected_tag=" << expected_tag
                          << ", token.str()=" << token.str() << std::endl;
                throw std::runtime_error("Invalid XML format");
            }

            break;
        }
        else if (token.type() == TokenType::TEXT_CONTENT) {
            XmlNode node(XmlNode::Type::TEXT_CONTENT, "", {}, {}, token.str());
            if (!root.name().empty()) {
                root.add_child(node);
            }
            else {
                root = std::move(node);
            }
        }

        ++it;
    }

    return root;
}
