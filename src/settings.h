#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Settings {
public:
    Settings(int argc, char **argv)
        : input_str{&std::cin}
        , output_str{&std::cout} {
        auto args = std::vector<std::string>{argv + 1, argv + argc};

        for (std::size_t i = 0; i < args.size(); ++i) {
            const auto &arg = args.at(i);

            if (arg == "--input" || arg == "-i") {
                if (i + 1 < args.size()) {
                    input_file = std::ifstream{args.at(i + 1)};
                    if (!input_file.is_open()) {
                        std::cerr << "Error: Could not open input file: "
                                  << args.at(i + 1) << std::endl;
                        std::exit(1);
                    }
                    input_str = &input_file;
                    ++i;
                }
                else {
                    std::cerr << "Error: No input file specified after: " << arg
                              << std::endl;
                    print_help_and_exit();
                }
            }
            else if (arg == "--output" || arg == "-o") {
                if (i + 1 < args.size()) {
                    output_file = std::ofstream{args.at(i + 1)};
                    if (!output_file.is_open()) {
                        std::cerr << "Error: Could not open output file: "
                                  << args.at(i + 1) << std::endl;
                        std::exit(1);
                    }
                    output_str = &output_file;
                    ++i;
                }
                else {
                    std::cerr
                        << "Error: No output file specified after: " << arg
                        << std::endl;
                    print_help_and_exit();
                }
            }
            else if (arg == "--help" || arg == "-h") {
                print_help_and_exit();
            }
            else {
                std::cerr << "Error: Unknown argument: " << arg << std::endl;
                print_help_and_exit();
            }
        }
    }

    std::istream &in() const {
        return *input_str;
    }

    std::ostream &out() const {
        return *output_str;
    }

private:
    void print_help_and_exit() const {
        std::cerr << R"HELP(
Usage: fastxml [options]

Options:
  --input, -i <filename>   Specify input file (default: stdin)
  --output, -o <filename>  Specify output file (default: stdout)
  --help, -h              Show this help message and exit
)HELP";
        std::exit(1);
    }

    std::ifstream input_file;
    std::ofstream output_file;
    std::istream *input_str;
    std::ostream *output_str;
};
