#include "settings.h"
#include "tinyxml2.h"
#include <iostream>
#include <sstream>
#include <utility>

int main(int argc, char **argv) {
    try {
        const auto settings = Settings{argc, argv};

        auto ss = std::stringstream{};

        ss << settings.in().rdbuf();
        auto str = std::move(ss).str();

        tinyxml2::XMLDocument doc;
        const auto parse_result = doc.Parse(str.c_str(), str.size());
        if (parse_result != tinyxml2::XML_SUCCESS) {
            throw std::runtime_error("Failed to parse XML file.");
        }

        auto printer = tinyxml2::XMLPrinter{};
        doc.Print(&printer);
        settings.out() << printer.CStr();
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
