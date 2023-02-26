#include "tvm.h"
#include "tvm_string.h"
#include <iostream>
#include <fstream>


void toy_virtual_machine::compile(std::filesystem::path p)
{
  std::ifstream src_file(p);
  if (!src_file.is_open())
	throw std::invalid_argument("Can't open source file '" + p.string() + "'.");

  // Прочитаю файл, пропуская ненужные строки
  std::vector<std::pair<std::size_t, std::string>> source;
  std::size_t num = 0;
  std::string line;
  while (++num, std::getline(src_file, line)) {
	tvm::trim_comments(line);
	tvm::trim(line);
	if (!line.empty())
	  source.push_back(std::make_pair(num, tvm::upper_case(line)));
  }

  for (auto& i : source) // тестовая печать
	std::cout << i.first << "\t|" << i.second << "|" << std::endl;


//  operand op1{operand::REGISTER, 0}, op2{operand::CONSTANT, 42};
//  rom_.push_back(std::make_unique<MOV>(op1, op2));
//
//  op2.type = operand::REGISTER;
//  op2.value = 0;
//  rom_.push_back(std::make_unique<ADD>(op1, op2));

  state_.PS = rom_.size();
}



void toy_virtual_machine::run()
{
  while (state_.PC != state_.PS)
	rom_[state_.PC]->execute(state_);
}



void toy_virtual_machine::print_state() const
{
  std::cout << "State of machine: " << std::endl;
  for (int i = 0, end = state_.R.size(); i < end; ++i)
	std::cout << "\tR" << i + 1 << " = " << static_cast<int>(state_.R[i]) << std::endl;
}
