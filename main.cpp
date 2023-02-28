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
    std::cout << "Create virtual machine... ";
    toy_virtual_machine tvm;
    std::cout << "Ok!" << std::endl;

    std::cout << "Compile source code... ";
    tvm.compile(argv[1]);
    std::cout << "Ok!" << std::endl;

    std::cout << "Execute code... ";
    tvm.run();
    std::cout << "Ok!" << std::endl;

    tvm.print_state();
  } catch (std::exception& e) {
    std::cout << "Fail!" << std::endl << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

