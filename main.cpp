#pragma hdrstop
#pragma argsused

#ifdef _WIN32
#include <tchar.h>
#else
  typedef char _TCHAR;
  #define _tmain main
#endif

#include <stdlib.h>
#include <exception>
#include <iostream>
#include "tvm.h"

int _tmain(int argc, _TCHAR* argv[]) 
{
  if (argc < 2) {
	std::cout << "Source code file not defined" << std::endl;
    system("pause");
	return EXIT_FAILURE;
  }


  try {
	toy_virtual_machine tvm;
	tvm.compile(argv[1]);
//	tvm.run();
	tvm.print_state();
  } catch (std::exception& e) {
	std::cout << e.what() << std::endl;
	system("pause");
	return EXIT_FAILURE;
  }


  system("pause");
  return EXIT_SUCCESS;
}
