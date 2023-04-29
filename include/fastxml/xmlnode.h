#pragma once

#include <charconv>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace fastxml {

class XmlNode {
public:
    enum class Type { ELEMENT, TEXT_CONTENT };

    struct Iterator {
        const XmlNode *ptr = 0;

        Iterator &operator++() {
            ptr = ptr->_next;
            return *this;
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
        if (_content.empty()) {
            if (_children && _children->type() == Type::TEXT_CONTENT) {
                return _children->str();
            }
        }
        return _content;
    }

    std::string_view str() const {
        return content();
    }

    /// Convert to a number
    template <typename T = int>
    T number() const {
        T ret = {};
        auto str = this->str();
        std::from_chars(str.data(), str.data() + str.size(), ret);

        return ret;
    }

    struct XmlAttribute {
        XmlAttribute() = default;
        XmlAttribute(const XmlAttribute &) = delete;
        XmlAttribute(XmlAttribute &&) = default;

        XmlAttribute(std::string_view name, std::string_view value)
            : name{name}
            , value{value} {}

        std::string_view name;
        std::string_view value;

        XmlAttribute *next = nullptr;

        /// Get the value as a string
        std::string_view str() const {
            return value;
        }

        /// Convert to a number
        template <typename T = int>
        T number() const {
            T ret = {};
            auto str = this->str();
            std::from_chars(str.data(), str.data() + str.size(), ret);
            return ret;
        }
    };

    struct XmlAttributes {

        struct Iterator {
            const XmlAttribute *ptr = 0;

            Iterator operator++() {
                ptr = ptr->next;
                return *this;
            }

            bool operator==(const Iterator &other) const {
                return ptr == other.ptr;
            }

            bool operator!=(const Iterator &other) const {
                return ptr != other.ptr;
            }

            const XmlAttribute &operator*() {
                return *ptr;
            }

            const XmlAttribute *operator->() {
                return ptr;
            }
        };

        XmlAttribute *_attributes = nullptr;

        const XmlAttribute *find(std::string_view name) const {
            for (const auto &attribute : *this) {
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

        bool empty() const {
            return !_attributes;
        }

        // Point first child
        void attributes(XmlAttribute *attribute) {
            _attributes = attribute;
        }

        Iterator begin() {
            return Iterator{_attributes};
        }

        Iterator end() {
            return Iterator{nullptr};
        }

        Iterator begin() const {
            return Iterator{_attributes};
        }

        Iterator end() const {
            return Iterator{nullptr};
        }

        const XmlAttribute &front() const {
            return *begin();
        }

        size_t size() const {
            auto count = size_t{};
            for (auto &attr : *this) {
                ++count;
            }
            return count;
        }
    };

    XmlNode(Type type, std::string_view name, std::string_view content = "")
        : _type(type)
        , _name(name)
        , _content(content) {}

    const XmlAttributes &attributes() const {
        return _attributes;
    }

    XmlAttributes &attributes() {
        return _attributes;
    }

    const XmlAttribute &attr(std::string_view name) const {
        return _attributes.at(name);
    }

    const XmlAttribute *findAttr(std::string_view name) const {
        return _attributes.find(name);
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

inline void xml_node_to_string(const XmlNode &node,
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

inline std::ostream &operator<<(std::ostream &os, const XmlNode &node) {
    xml_node_to_string(node, os);
    return os;
}

} // namespace fastxml
