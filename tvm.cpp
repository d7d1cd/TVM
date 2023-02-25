#include "tvm.h"
#include <iostream>
#include <fstream>
#include <vector>


void toy_virtual_machine::compile(std::filesystem::path p)
{
  std::ifstream src_file(p);
  if (!src_file.is_open())
	throw std::invalid_argument("Can't open source file '" + p.string() + "'.");

  std::vector<std::string> list;
  std::string line;
  while (std::getline(src_file, line))
	list.push_back(line);

  for (auto& s : list)
	std::cout << "'" << s << "'" << std::endl;
}



void toy_virtual_machine::print_state() const
{
  std::cout << "State of machine: " << std::endl
			<< "\tR1 = " << static_cast<int>(R1) << std::endl
			<< "\tR2 = " << static_cast<int>(R2) << std::endl
			<< "\tR3 = " << static_cast<int>(R3) << std::endl
			<< "\tR4 = " << static_cast<int>(R4) << std::endl;
}
