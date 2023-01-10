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
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "produce help message")
            ("compression", po::value<double>(), "set compression level");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 0;
    }
//  arg_parser test{argc, argv};

  std::cout << "Has " << argc << " arguments:" << std::endl;
  for (int i = 0; i < argc; ++i) {
      std::cout << argv[i] << std::endl;
  }
  return 0;
}