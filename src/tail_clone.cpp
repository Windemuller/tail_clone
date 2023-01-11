#include <iostream>
#include <boost/program_options.hpp>
#include <fstream>
#include "arg_parser.h"

#define DEFAULT_NEW_LINE_COUNT 10

namespace po = boost::program_options;

/**
  Options:
  * -c, --bytes=[+]NUM
  * -n, --lines=[+]NUM
  * -f, --follow
  * -s, --sleep-interval=N
  * --help
  * --version
*/

void read_file_test(std::string filename, int line_count) {
    // Open the file
    std::ifstream file{};
    file.open(filename, std::ios::ate);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(1);
    }

    // Get the current position of the file. Because we opened it with mode ate, it is at the end of the file.
    std::streampos file_end_pos = file.tellg();

    int newlines = 0;
    char out;
    for (int i = 1; i <= file_end_pos; i++) {
        // Move the file pointer back one character
        file.seekg(-i, std::ios::end);
        file.get(out);
        if (i != 1 && out == '\n') {
            newlines++;
        }
        if (newlines == line_count) {
            break;
        }
    }
    if (newlines < line_count) {
        // We didn't find 10 newlines, so we need to start at the beginning of the file
        file.seekg(0, std::ios::beg);
    }

    auto file_pos = file.tellg();
    // Output everything from the current position to the end of the file
    for (int i = file_pos; i < file_end_pos; i++) {
        file.seekg(i, std::ios::beg);
        file.get(out);
        std::cout << out;
    }
}

int main(int argc, char *argv[]) {
    try {
        po::options_description desc(
"Usage: tail_clone [OPTION]... [FILE]...\n"
"Print the last 10 lines of each FILE to standard output.\n\n"
"With no FILE, read standard input.\n\n"
"Arguments");
        desc.add_options()
                ("bytes", po::value<int>()->value_name("[+]NUM"), "output the last NUM bytes")
                ("lines", po::value<int>()->value_name("[+]NUM")->default_value(DEFAULT_NEW_LINE_COUNT), "output the last NUM lines")
                ("follow,f", po::bool_switch(), "output appended data as the file grows")
                ("sleep-interval,s", po::value<int>()->value_name("NUM")->default_value(1), "with -f, sleep for approximately NUM seconds (default 1.0) between iterations")
                ("help", "display this help and exit")
                ("version", "output version information and exit")
                ("file", "the file to read from");

        // Add position argument "file"
        po::positional_options_description p;
        p.add("file", 1);

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 0;
        }
        if (vm.count("file")) {
            // If there is a file defined, read it
            int line_count = vm["lines"].as<int>();
            read_file_test(vm["file"].as<std::string>(), line_count);
        } else {
            // There is no file, read from stdin
            std::string input;
            while (std::getline(std::cin, input)) { // TODO: Read from bottom
                std::cout << input << std::endl;
            }
            std::cout << std::endl;
        }
//        if (vm.count("bytes")) {
//            std::cout << "Bytes was set to "
//                      << vm["bytes"].as<int>() << ".\n";
//        } else {
//            std::cout << "Bytes was not set.\n";
//        }
    } catch(std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    } catch(...) {
        std::cerr << "Error!\n";
    }
    return 0;
}