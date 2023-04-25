#pragma once

#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

class XmlNode {
public:
    enum class Type { ELEMENT, TEXT_CONTENT };

    struct Iterator {
        const XmlNode *ptr = 0;

        void operator++() {
            ptr = ptr->_next;
        }

        bool operator==(const Iterator &other) const {
            return ptr == other.ptr;
        }

        bool operator!=(const Iterator &other) const {
            return ptr != other.ptr;
        }

        const XmlNode &operator*() {
            return *ptr;
        }

        const XmlNode *operator->() {
            return ptr;
        }
    };

    Type type() const {
        return _type;
    }

    void name(std::string_view name) {
        _name = name;
    }

    std::string_view name() const {
        return _name;
    }

    Iterator begin() const {
        return Iterator{_children};
    }

    Iterator end() const {
        return {};
    }

    void content(std::string_view value) {
        _content = value;
    }

    std::string_view content() const {
        return _content;
    }

    struct XmlAttribute {
        std::string_view name;
        std::string_view value;

        XmlAttribute *next = nullptr;
    };
    struct XmlAttributes {
        std::vector<XmlAttribute> attributes;

        XmlAttribute *find(std::string_view name) {
            for (auto &attribute : attributes) {
                if (attribute.name == name) {
                    return &attribute;
                }
            }
            return nullptr;
        }

        const XmlAttribute *find(std::string_view name) const {
            for (const auto &attribute : attributes) {
                if (attribute.name == name) {
                    return &attribute;
                }
            }
            return nullptr;
        }

        XmlAttribute &operator[](std::size_t index) {
            return attributes[index];
        }

        const XmlAttribute &operator[](std::size_t index) const {
            return attributes[index];
        }

        std::size_t size() const {
            return attributes.size();
        }

        void clear() {
            attributes.clear();
        }

        bool empty() const {
            return attributes.empty();
        }

        void push_back(XmlAttribute attribute) {
            attributes.push_back(std::move(attribute));
        }

        auto begin() {
            return attributes.begin();
        }

        auto end() {
            return attributes.end();
        }

        auto begin() const {
            return attributes.begin();
        }

        auto end() const {
            return attributes.end();
        }
    };

    XmlNode(Type type,
            std::string_view name,
            XmlAttributes attributes = {},
            std::string_view content = "")
        : _type(type)
        , _name(name)
        , _attributes(std::move(attributes))
        , _content(content) {}

    const XmlAttributes &attributes() const {
        return _attributes;
    }

    XmlAttributes &attributes() {
        return _attributes;
    }

    void children(XmlNode *firstChild) {
        if (_children) {
            throw std::runtime_error{"Xml error: Child is already set"};
        }
        _children = firstChild;
    }

    void next(XmlNode *next) {
        _next = next;
    }

private:
    Type _type;
    std::string_view _name;
    std::string_view _content;
    XmlAttributes _attributes;
    XmlNode *_children = nullptr;
    XmlNode *_next = nullptr;
};

void xml_node_to_string(const XmlNode &node,
                        std::ostream &stream,
                        std::size_t indent = 0) {
    if (node.type() == XmlNode::Type::ELEMENT) {
        // Print the opening tag
        stream << std::string(indent, ' ') << "<" << node.name();

        // Print the attributes, if any
        const auto &attributes = node.attributes();
        for (const auto &attribute : attributes) {
            stream << " " << attribute.name << "=\"" << attribute.value << "\"";
        }

        // Print the closing tag and content
        if (node.content().empty() && node.begin() == node.end()) {
            // Empty tag
            stream << "/>\n";
        }
        else {
            // Non-empty tag
            stream << ">\n";

            // Print the children
            for (const auto &child : node) {
                xml_node_to_string(child, stream, indent + 2);
            }

            // Print the closing tag
            stream << std::string(indent, ' ') << "</" << node.name() << ">\n";
        }
    }
    else if (node.type() == XmlNode::Type::TEXT_CONTENT) {
        // Print the content
        stream << std::string(indent, ' ') << node.content() << "\n";
    }
}

std::ostream &operator<<(std::ostream &os, const XmlNode &node) {
    xml_node_to_string(node, os);
    return os;
}
