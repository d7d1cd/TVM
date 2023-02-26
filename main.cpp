#include <stdlib.h>
#include <exception>
#include <iostream>
#include "tvm.h"

int main(int argc, char* argv[]) 
{
  if (argc < 2) {
    std::cout << "Source code file not defined" << std::endl;
    return EXIT_FAILURE;
  }

  try {
    toy_virtual_machine tvm;
    tvm.compile(argv[1]);
    tvm.run();
    tvm.print_state();
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

