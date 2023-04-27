#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
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
                    in_path = args.at(++i);
                    input_file = std::ifstream{in_path};
                    if (!input_file.is_open()) {
                        std::cerr
                            << "Error: Could not open input file: " << in_path
                            << std::endl;
                        std::exit(1);
                    }
                    input_str = &input_file;
                    check_if_same();
                }
                else {
                    std::cerr << "Error: No input file specified after: " << arg
                              << std::endl;
                    print_help_and_exit();
                }
            }
            else if (arg == "--output" || arg == "-o") {
                if (i + 1 < args.size()) {
                    out_path = args.at(++i);
                    output_file = std::ofstream{out_path};
                    if (!output_file.is_open()) {
                        std::cerr
                            << "Error: Could not open output file: " << out_path
                            << std::endl;
                        std::exit(1);
                    }
                    output_str = &output_file;
                    check_if_same();
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

    void check_if_same() {
        if (input_str == &tmp) {
            return;
        }
        if (in_path == out_path) {
            tmp << input_str->rdbuf();
            input_str = &tmp;
        }
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

    std::filesystem::path in_path;
    std::filesystem::path out_path;

    std::stringstream tmp; // Used if input and output is the same

    std::ifstream input_file;
    std::ofstream output_file;
    std::istream *input_str;
    std::ostream *output_str;
};
