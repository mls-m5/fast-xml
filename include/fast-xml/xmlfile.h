#pragma once

#include <istream>
#include <sstream>
#include <string>
#include <string_view>

class XmlFile {
public:
    class Reader {
    public:
        Reader &get(const char *&ch) {
            if (!*this) {
                return *this;
            }
            status = position < content.size();
            if (status) {
                ch = &content.at(position++);
            }
            return *this;
        }

        int peek() const {
            if (!*this) {
                return 0;
            }
            return content.at(position);
        }

        operator bool() const {
            return status;
        };

        Reader(std::string_view content)
            : content{content} {}

    private:
        int position = 0;
        std::string_view content;
        bool status = true;
    };

public:
    XmlFile(std::string content)
        : _content{content} {}

    XmlFile(std::istream &stream) {
        auto ss = std::stringstream{};
        ss << stream.rdbuf();
        _content = std::move(ss).str();
    }

    ~XmlFile() = default;

    XmlFile() = delete;
    XmlFile(const XmlFile &) = delete;
    XmlFile(XmlFile &&) = delete;
    XmlFile &operator=(const XmlFile &) = delete;
    XmlFile &operator=(XmlFile &&) = delete;

    std::string_view content() {
        return _content;
    }

    Reader reader() const {
        return Reader{_content};
    }

    bool isInFile(std::string_view str) {
        return str.data() >= _content.data() &&
               str.data() + str.size() < _content.data() + _content.size();
    }

private:
    std::string _content;
};
