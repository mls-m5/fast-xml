#pragma once

#include "xml_tokenizer.h"
#include "xmlnode.h"
#include <stack>

XmlNode parse(std::istream &input) {
    std::vector<XmlToken> tokens = tokenize(input);

    // Use a stack to keep track of nested elements
    std::stack<XmlNode> node_stack;

    for (const XmlToken &token : tokens) {
        switch (token.type()) {
        case TokenType::ELEMENT_OPEN: {
            // Create a new XmlNode and add it to the parent node
            XmlNode node(XmlNode::Type::ELEMENT, token.str());
            if (!node_stack.empty()) {
                node_stack.top().add_child(node);
            }
            node_stack.push(node);

            // Add attributes to the node
            XmlNode::XmlAttributes &attributes = node.attributes();
            while (tokens.front().type() == TokenType::ATTRIBUTE_NAME) {
                XmlToken name_token = tokens.front();
                tokens.erase(tokens.begin());
                XmlToken value_token = tokens.front();
                tokens.erase(tokens.begin());
                attributes.push_back(
                    XmlNode::XmlAttribute{name_token.str(), value_token.str()});
            }
            break;
        }
        case TokenType::ELEMENT_CLOSE: {
            // Pop the last element off the stack
            node_stack.pop();
            break;
        }
        case TokenType::TEXT_CONTENT: {
            // Create a new XmlNode with the text content and add it to the
            // parent node
            XmlNode node(XmlNode::Type::TEXT_CONTENT, "", {}, {}, token.str());
            if (!node_stack.empty()) {
                node_stack.top().add_child(node);
            }
            break;
        }
        default:
            // Ignore all other tokens
            break;
        }
    }

    // The root node is the only node left in the stack
    return node_stack.top();
}
