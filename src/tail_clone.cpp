#include <iostream>
#include <boost/program_options.hpp>
#include "arg_parser.h"

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

int main(int argc, char *argv[]) {
    try {
        po::options_description desc(
"Usage: tail_clone [OPTION]... [FILE]...\n"
"Print the last 10 lines of each FILE to standard output.\n\n"
"With no FILE, read standard input.\n\n"
"Arguments");
        desc.add_options()
                ("bytes", po::value<int>()->value_name("[+]NUM"), "output the last NUM bytes")
                ("lines", po::value<int>()->value_name("[+]NUM")->default_value(10), "output the last NUM lines")
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
            std::cout << "File:";
            std::cout << vm["file"].as<std::string>() << "\n";
        } else {
            // There is no file, read from stdin
            std::string input;
            while (std::getline(std::cin, input)) {
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