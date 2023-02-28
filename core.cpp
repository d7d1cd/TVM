#include "core.h"


void operand::parse(const std::string& s)
{
  if (s.empty())
	throw std::runtime_error("Empty operator");

  // Регистр - это R и одна цифра от 1 до 4
  else if (s.size() == 2 && s[0] == 'R' && std::isdigit(s[1])) {
	int v = s[1] - '0';
	if (v < 1 || v > 4)
	  throw std::runtime_error("Invalid register number");
	type = REGISTER;
	value = v - 1;
  }

  // Адрес - это все, что начинается на [ и длиннее 2- символов
  else if (s.size() > 2 && s.front() == '[') {
	if (s.back() != ']' || !tvm::is_number({&s[1], s.size() - 2}))
	  throw std::runtime_error("Invalid memory address format");

	int v = std::stoi(std::string(&s[1], s.size() - 2));
	if (v < 0 || v > 255)
	  throw std::out_of_range("Illegal memory address");
	type = ADDRESS;
	value = v;
  }

  // Числовой литерал - это просто число
  else if (tvm::is_number(s)) {
	int v = std::stoi(s);
	if (v < 0 || v > 255)
	  throw std::out_of_range("Illegal constant value");
	type = CONSTANT;
	value = v;
  }

  // Метка - это просто слово
  else if (tvm::is_word(s)) {
	type = LABEL;
	value = 0;
  }

  // Все остальное - что-то непонятное
  else
	throw std::runtime_error("Invalid operand '" + s + "'");
}


std::unique_ptr<instruction> compiler::create_instruction(std::string i)
{
  if (i == "HLT")
	return std::make_unique<HLT>();
  throw std::runtime_error("Invalid instruction: " + i);
}


std::unique_ptr<instruction> compiler::create_instruction(std::string i, operand op1)
{
  if (i == "PUSH")
	return std::make_unique<PUSH>(op1);
  if (i == "POP")
	return std::make_unique<POP>(op1);
  if (i == "JMP")
	return std::make_unique<JMP>(op1);


  throw std::runtime_error("Invalid instruction: " + i);
}


std::unique_ptr<instruction> compiler::create_instruction(std::string i, operand op1, operand op2)
{
  if (i == "MOV")
	return std::make_unique<MOV>(op1, op2);
  if (i == "ADD")
	return std::make_unique<ADD>(op1, op2);
  if (i == "SUB")
	return std::make_unique<SUB>(op1, op2);

  throw std::runtime_error("Invalid instruction: " + i);
}


std::unique_ptr<instruction> compiler::make_instruction(std::size_t l, std::vector<std::string> t)
{
  if (t.size() == 1)
	return create_instruction(t[0]);

  operand op1(t[1]);
  if (t.size() == 2)
	return create_instruction(t[0], op1);

  if (op1.type == operand::LABEL)
	throw std::runtime_error("\tLine " + std::to_string(l) + ": Label in two operand instruction");

  operand op2(t[2]);
  return create_instruction(t[0], op1, op2);
}
