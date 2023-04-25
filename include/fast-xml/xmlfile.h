#pragma once

#include <istream>
#include <sstream>
#include <string>
#include <string_view>

class XmlFile {
    class Reader {
    public:
        int get() {
            if (!*this) {
                return 0;
            }
            return content.at(position++);
        }

        int peek() const {
            if (!*this) {
                return 0;
            }
            return content.at(position);
        }

        operator bool() const {
            return position < content.size();
        };

        Reader(std::string_view content)
            : content{content} {}

    private:
        int position = 0;
        std::string_view content;
    };

public:
    XmlFile(std::string content)
        : _content{content} {}

    XmlFile(std::istream &stream) {
        auto ss = std::stringstream{};
        ss << stream.rdbuf();
        _content = std::move(ss).str();
    }

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

private:
    std::string _content;
};