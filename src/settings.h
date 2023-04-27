#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class Settings {
public:
    Settings(int argc, char **argv) {
        auto args = std::vector<std::string>{argv + 1, argv + argc};

        for (std::size_t i = 0; i < args.size(); ++i) {
            const auto &arg = args.at(i);

            if (arg == "--input" || arg == "-i") {
                in_path = args.at(++i);
                input_file = std::ifstream{in_path};
                if (!input_file.is_open()) {
                    std::cerr << "Error: Could not open input file: " << in_path
                              << std::endl;
                    std::exit(1);
                }
                input_str = &input_file;
                check_if_same();
            }
            else if (arg == "--output" || arg == "-o") {
                out_path = args.at(++i);
            }
            else if (arg == "--help" || arg == "-h") {
                print_help_and_exit();
            }
            else {
                std::cerr << "Error: Unknown argument: " << arg << std::endl;
                print_help_and_exit();
            }
        }

        // Must be last to make sure that nothing is written before the content
        // is possibly copied
        if (!out_path.empty()) {
            check_if_same();
            output_file = std::ofstream{out_path};
            if (!output_file.is_open()) {
                std::cerr << "Error: Could not open output file: " << out_path
                          << std::endl;
                std::exit(1);
            }
            output_str = &output_file;
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
        if (std::filesystem::equivalent(in_path, out_path)) {
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
    std::istream *input_str = &std::cin;
    std::ostream *output_str = &std::cout;
};
