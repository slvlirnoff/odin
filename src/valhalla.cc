#include <iostream>
#include <string>
#include <vector>
#include <boost/program_options.hpp>

#include "config.h"

namespace bpo = boost::program_options;

int main(int argc, char *argv[])
{

  bpo::options_description options(
  "valhalla " VERSION "\n"
  "\n"
  " Usage: valhalla [options] <protocolbuffer_input_file>\n"
  "\n"
  "valhalla is simply a program that fronts a stubbed out library in the "
  "autotools framework. The purpose of it is to have a standard configuration "
  "for any new project that one might want to do using autotools and c++11. "
  "It includes some pretty standard dependencies for convenience but those "
  "may or may not be actual requirements for valhalla to build. "
  "\n"
  "\n");


  std::string echo;

  options.add_options()
    ("help,h", "Print this help message.")
    ("version,v", "Print the version of this software.")
    // positional arguments
    ("echo", bpo::value<std::string>(&echo), "String for the application to echo back")
    ;


  bpo::positional_options_description pos_options;
  pos_options.add("echo", 1);

  bpo::variables_map vm;

  try {
    bpo::store(bpo::command_line_parser(argc,argv)
      .options(options)
      .positional(pos_options)
      .run(),
      vm);
    bpo::notify(vm);

  } catch (std::exception &e) {
    std::cerr << "Unable to parse command line options because: " << e.what() << "\n" << "This is a bug, please report it at " PACKAGE_BUGREPORT << "\n";
    return EXIT_FAILURE;
  }

  if (vm.count("help")) {
    std::cout << options << "\n";
    return EXIT_SUCCESS;
  }

  if (vm.count("version")) {
    std::cout << "valhalla " << VERSION << "\n";
    return EXIT_SUCCESS;
  }

  // argument checking and verification
  for (auto arg : {"echo"}) {
    if (vm.count(arg) == 0) {
      std::cerr << "The <" << arg << "> argument was not provided, but is mandatory\n\n";
      std::cerr << options << "\n";
      return EXIT_FAILURE;
    }
  }

  //echo it back
  std::cout << echo << "\n";

  return EXIT_SUCCESS;
}