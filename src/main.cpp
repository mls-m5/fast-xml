#include "fast-xml/xml_parser.h"
#include <iostream>

int main() {
    try {
        std::cout << parse(std::cin) << std::endl;
    }
    catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
