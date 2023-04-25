#pragma once

#include <string>
#include <string_view>
#include <vector>

class XmlNode {
public:
    enum class Type { ELEMENT, TEXT_CONTENT };

    Type type() const {
        return _type;
    }

    std::string_view name() const {
        return _name;
    }

    std::vector<XmlNode>::const_iterator begin() const {
        return _children.cbegin();
    }

    std::vector<XmlNode>::const_iterator end() const {
        return _children.cend();
    }

    std::string_view content() const {
        return _content;
    }

    struct XmlAttribute {
        std::string_view name;
        std::string_view value;
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
    };

    XmlNode(Type type,
            std::string_view name,
            std::vector<XmlNode> children = {},
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

    void add_child(XmlNode child) {
        _children.push_back(std::move(child));
    }

private:
    Type _type;
    std::string _name;
    std::vector<XmlNode> _children;
    std::string _content;
    XmlAttributes _attributes;
};
