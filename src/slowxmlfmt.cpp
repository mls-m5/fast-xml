#include "fast-xml/heap_xml_parser.h"
#include "settings.h"

using namespace fastxml;

int main(int argc, char **argv) {
    try {
        const auto settings = Settings{argc, argv};
        settings.out() << parseSlow(settings.in()) << std::endl;
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
