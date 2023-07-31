#pragma once

#include <charconv>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

namespace fastxml {

/// XmlNode for fast output
class OutXmlNode {
public:
    enum class Type { Element, TextContent };
    enum class Status { Attributes, Content };

    void content(std::string_view value) {
        _has_children = true;
        finish_attributes();
        indent();
        _stream << value;
    }

    template <typename T>
    void add_attribute(std::string_view name, const T &value) {
        _stream << " " << name << " = \"" << value << '"';
    }

    OutXmlNode(std::ostream &stream,
               std::string_view name,
               Type type = Type::Element,
               int indentation = 0,
               char newLine = '\n')
        : _stream{stream}
        , _name{name}
        , _type{type} {
        if (_type == Type::TextContent) {
            _stream << name;
            return;
        }
        _stream << "<" << name;
    }

    // Save this value. The destructor handles closing
    OutXmlNode add_child(std::string_view name, Type type = Type::Element) {
        _has_children = true;
        if (_status == Status::Attributes) {
            finish_attributes();
        };

        return OutXmlNode{
            _stream, name, type, _indentation + 1, _newline_character};
    }

    ~OutXmlNode() {
        if (_type == Type::TextContent) {
            return;
        }
        finish_attributes();
        if (_has_children) {
            indent();
            _stream << "</" << _name << ">\n";
        }
    }

private:
    void finish_attributes() {
        if (_type == Type::TextContent) {
            return;
        }
        if (_status == Status::Content) {
            return;
        }
        _status = Status::Content;
        _stream << ">";
    }

    void indent() {
        if (_newline_character) {
            _stream << _newline_character;
            for (int i = 0; i < _indentation; ++i) {
                _stream << "  ";
            }
        }
    }

    std::ostream &_stream;
    std::string _name;
    bool _has_children = false;
    Type _type;
    int _indentation = 0;
    Status _status = Status::Attributes;
    char _newline_character = '\n';
};

} // namespace fastxml
