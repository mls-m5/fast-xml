#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "settings.h"
#include <fstream>
#include <sstream>
#include <vector>

std::string parse(std::istream &in) {
    // This seems to be slighty slower
    //    std::vector<char> xml_data((std::istreambuf_iterator<char>(in)),
    //                               std::istreambuf_iterator<char>());
    //    xml_data.push_back('\0');

    auto iss = std::ostringstream{};
    iss << in.rdbuf();
    auto xml_data = std::move(iss).str();

    try {
        rapidxml::xml_document<> doc;
        doc.parse<0>(xml_data.data());

        std::ostringstream out;
        out << doc;
        return out.str();
    }
    catch (const rapidxml::parse_error &e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
        throw;
    }
}

int main(int argc, char **argv) {
    try {
        const auto settings = Settings{argc, argv};
        settings.out() << parse(settings.in()) << std::endl;
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
