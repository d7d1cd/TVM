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

  operand op1, op2;
  compiler cmpl;
  std::unique_ptr<instruction> ins;

  for (auto& s : source) {
	// Токенизируем строку, проверим корректность
	auto t = tvm::tokenize(s.second, " ,");
	if (t.size() > 3)
	  throw std::runtime_error("\tLine " + std::to_string(s.first) + ": Too many identifiers");

	// Создадим операнды из токенов
	if (t.size() == 1) {
	  ins = cmpl.create_instruction(t[0]);
	} else if (t.size() == 2) {
	  op1.parse(t[1]);
	  ins = cmpl.create_instruction(t[0], op1);
	} else {
	  op1.parse(t[1]);
	  op2.parse(t[2]);
	  ins = cmpl.create_instruction(t[0], op1, op2);
	}

	rom_.push_back(std::move(ins));
  }

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
