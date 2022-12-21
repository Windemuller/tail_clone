#include <iostream>
#include "inc/arg_parser.h"

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
  arg_parser test{argc, argv};
  test.

  std::cout << "Have " << argc << " arguments:" << std::endl;
  for (int i = 0; i < argc; ++i) {
      std::cout << argv[i] << std::endl;
  }
  return 0;
}