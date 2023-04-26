#pragma once

#include <charconv>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

namespace fastxml {

/// Xml node that use heap allocations
/// This could be used when writing xml data, but note that using it might be
/// slower than other alternatives
class HeapXmlNode {
public:
    enum class Type { ELEMENT, TEXT_CONTENT };

    Type type() const {
        return _type;
    }

    void name(std::string_view name) {
        _name = std::string{name};
    }

    std::string_view name() const {
        return _name;
    }

    std::vector<HeapXmlNode>::const_iterator begin() const {
        return _children.cbegin();
    }

    std::vector<HeapXmlNode>::const_iterator end() const {
        return _children.cend();
    }

    void content(std::string_view value) {
        _content = std::string{value};
    }

    std::string_view content() const {
        return _content;
    }

    struct XmlAttribute {
        std::string_view name;
        std::string_view value;

        /// Get the value as a string
        std::string_view str() const {
            return value;
        }

        /// Convert to a number
        template <typename T = int>
        T number() const {
            T ret = {};
            auto str = this->str();
            std::from_chars(str.begin(), str.end(), ret);
            return ret;
        }
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

        const XmlAttribute &at(std::string_view name) const {
            if (auto f = find(name)) {
                return *f;
            }
            throw std::out_of_range{"could not find attribute " +
                                    std::string{name}};
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

        auto &front() const {
            return attributes.front();
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

    HeapXmlNode(Type type,
                std::string_view name,
                std::vector<HeapXmlNode> children = {},
                XmlAttributes attributes = {},
                std::string_view content = "")
        : _type(type)
        , _name(name)
        , _children(std::move(children))
        , _attributes(std::move(attributes))
        , _content(content) {}

    const XmlAttributes &attributes() const {
        return _attributes;
    }

    XmlAttributes &attributes() {
        return _attributes;
    }

    void add_child(HeapXmlNode child) {
        _children.push_back(std::move(child));
    }

private:
    Type _type;
    std::string _name;
    std::vector<HeapXmlNode> _children;
    std::string _content;
    XmlAttributes _attributes;
};

void xml_node_to_string(const HeapXmlNode &node,
                        std::ostream &stream,
                        std::size_t indent = 0) {
    if (node.type() == HeapXmlNode::Type::ELEMENT) {
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
    else if (node.type() == HeapXmlNode::Type::TEXT_CONTENT) {
        // Print the content
        stream << std::string(indent, ' ') << node.content() << "\n";
    }
}

std::ostream &operator<<(std::ostream &os, const HeapXmlNode &node) {
    xml_node_to_string(node, os);
    return os;
}

} // namespace fastxml
