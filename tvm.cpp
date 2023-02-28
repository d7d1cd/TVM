#include "tvm.h"
#include "tvm_string.h"
#include <iostream>
#include <fstream>
#include <unordered_map>



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


  // Компиляция
  compiler cmpl;
  std::unordered_map<std::string, std::size_t> labels;
  std::vector<std::tuple<std::size_t, std::size_t, std::string>> fix_instr;

  for (auto& s : source) {
	auto t = tvm::tokenize(s.second, " ,\t");

	// Проверим на "меткость"
	if (tvm::is_label(t[0])) {
	  labels[t[0]] = rom_.size();
	  t.erase(t.begin());
	  if (t.empty())
		continue;
	}

    if (t.size() > 3)
	  throw std::runtime_error("\tLine " + std::to_string(s.first) + ": Too many identifiers");

	// Попробуем создать инструкцию
	auto instr = cmpl.make_instruction(s.first, t);
	auto tpi_ptr = dynamic_cast<one_parm_instruction*>(instr.get());
	if (tpi_ptr && tpi_ptr->operand_access().type == operand::LABEL)
	  fix_instr.push_back({s.first, rom_.size(), t[1]});

	rom_.push_back(std::move(instr));
  }


  // Замена меток на реальные адреса
  for (auto& instr : fix_instr) {
	auto label = std::get<2>(instr);
	auto value = labels.find(label);
	if (value == labels.end())
	  throw std::runtime_error("\tLine " + std::to_string(std::get<0>(instr)) +
		": Label '" + label + "' not found");

	auto& i = rom_[std::get<1>(instr)];
	auto ptr = dynamic_cast<one_parm_instruction*>(i.get());
	ptr->operand_access().value = value->second;
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
