#pragma once

#include "tokentype.h"
#include "xml_tokenizer.h"
#include "xmlfile.h"
#include "xmlnode.h"
#include "xmltoken.h"
#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <string_view>

struct XmlRoot {
private:
    std::vector<XmlNode> nodes;
    std::vector<XmlNode::XmlAttribute> attributes;

public:
    void reserve(int n, int a) {
        nodes.reserve(n);
        attributes.reserve(a);
    }

    XmlNode &createNode(XmlNode::Type type, std::string_view content) {
        nodes.emplace_back(type, content);
        return nodes.back();
    }

    XmlNode::XmlAttribute &createAttribute(std::string_view name,
                                           std::string_view value) {
        attributes.push_back({name, value});
        return attributes.back();
    }

    // This is required to be saved since the tokens' and nodes' string_views
    // points to the content
    std::shared_ptr<XmlFile> file;

    auto &root() const {
        return nodes.front();
    }
};

std::ostream &operator<<(std::ostream &os, const XmlRoot &x) {
    return os << x.root();
}

XmlNode &parse(std::vector<XmlToken>::const_iterator &it,
               std::vector<XmlToken>::const_iterator end,
               XmlRoot &root) {

    if (it->type() == TokenType::TEXT_CONTENT) {
        XmlNode &node = root.createNode(XmlNode::Type::TEXT_CONTENT, {});
        node.content(it->str());
        ++it;
        return node;
    }

    if (it->type() != TokenType::ELEMENT_OPEN) {
        throw std::runtime_error{"expected opening tag"};
    }

    XmlNode &node = root.createNode(XmlNode::Type::ELEMENT, "");

    if (!root.file->isInFile(it->str())) {
        throw std::runtime_error{"str is not in file"};
    }
    node.name(it->str());
    ++it;

    XmlNode::XmlAttribute *lastAttribute = nullptr;

    // Parse the attributes
    while (it != end && it->type() == TokenType::ATTRIBUTE_NAME) {
        auto attribute_name = it->str();
        if ((it + 1) == end || (it + 1)->type() != TokenType::ATTRIBUTE_VALUE) {
            throw std::runtime_error(
                "Invalid XML format: Missing attribute value");
        }
        ++it;
        auto attribute_value = it->str();
        auto &attr = root.createAttribute(attribute_name, attribute_value);
        if (lastAttribute) {
            lastAttribute->next = &attr;
        }
        else {
            node.attributes().attributes(&attr);
        }
        lastAttribute = &attr;
        ++it;
    }

    XmlNode *lastChild = nullptr;

    auto addChild = [&](XmlNode *child) {
        if (lastChild) {
            lastChild->next(child);
            lastChild = child;
            return;
        }
        node.children(child);
        lastChild = child;
    };

    while (it != end) {
        const auto &token = *it;
        if (token.type() == TokenType::ELEMENT_OPEN) {
            // Recursively parse the children
            while (it != end && it->type() != TokenType::ELEMENT_CLOSE) {
                addChild(&parse(it, end, root));
            }
            return node;
        }
        else if (token.type() == TokenType::ELEMENT_CLOSE) {
            ++it;
            return node;
        }
        else if (token.type() == TokenType::TEXT_CONTENT) {
            addChild(&parse(it, end, root));
        }
        else {
            throw std::runtime_error{"Invalid token"};
        }
    }

    throw std::runtime_error("Invalid XML format: Missing element closing tag");
}

XmlRoot parse(std::istream &input) {
    int numNodes = 0;
    int numAttributes = 0;

    auto root = XmlRoot{};

    root.file = std::make_shared<XmlFile>(input);

    auto reader = root.file->reader();
    std::vector<XmlToken> tokens = tokenize(reader);

    for (auto &token : tokens) {
        if (token.type() == TokenType::ELEMENT_OPEN ||
            token.type() == TokenType::TEXT_CONTENT) {
            ++numNodes;
        }
        else if (token.type() == TokenType::ATTRIBUTE_NAME) {
            ++numAttributes;
        }
    }

    root.reserve(numNodes, numAttributes);

    auto it = tokens.cbegin();
    auto node = parse(it, tokens.cend(), root);
    return root;
}
