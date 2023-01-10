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
        po::options_description desc("Allowed options");
        desc.add_options()
                ("bytes", po::value<int>()->value_name("[+]NUM"), "output the last NUM bytes")
                ("lines", po::value<int>()->value_name("[+]NUM")->default_value(10), "output the last NUM lines")
                ("follow,f", po::bool_switch(), "output appended data as the file grows")
                ("sleep-interval,s", po::value<int>()->value_name("NUM")->default_value(1), "with -f, sleep for approximately NUM seconds (default 1.0) between iterations")
                ("help", "display this help and exit")
                ("version", "output version information and exit");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 0;
        }
        if (vm.count("bytes")) {
            std::cout << "Bytes was set to "
                      << vm["bytes"].as<int>() << ".\n";
        } else {
            std::cout << "Bytes was not set.\n";
        }
    } catch(std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    }

    std::cout << "Has " << argc << " arguments:" << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << std::endl;
    }
    return 0;
}