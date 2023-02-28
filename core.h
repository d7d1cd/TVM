#ifndef coreH
#define coreH

#include "tvm_string.h"
#include <array>
#include <memory>
#include <stdexcept>


struct vm_state
{
  std::size_t PC = 0;
  std::size_t SP = 256;
  std::size_t PS = 0;
  std::array<char, 4> R = {};
  std::array<char, 256> RAM = {};
};


struct operand
{
  operand() = default;
  operand(const std::string& s) {
	parse(s);
  }

  void parse(const std::string& s);

  char& map_to_state(vm_state& state) {
	switch (type) {
	  case REGISTER: return state.R[value];
	  case ADDRESS:  return state.RAM[value];
	  case CONSTANT:
	  case LABEL:    return value;
	  default:
		throw std::runtime_error("Unknown operand type");
	}
  }

  enum { REGISTER, ADDRESS, CONSTANT, LABEL } type = REGISTER;
  char value = 0;
};


struct instruction
{
  virtual void execute(vm_state&) = 0;
  virtual ~instruction() {}
};


class one_parm_instruction : public instruction
{
  public:
  one_parm_instruction(operand op1)
  : op1_{op1} {}

  operand& operand_access() {
	return op1_;
  }

  protected:
  char& destination(vm_state& state) {
	return op1_.map_to_state(state);
  }

  private:
  operand op1_;
};


class two_parm_instruction : public one_parm_instruction
{
  public:
  two_parm_instruction(operand op1, operand op2)
  : one_parm_instruction(op1), op2_{op2} {}

  operand& operand_access() {
	return op2_;
  }

  protected:
  char source(vm_state& state) {
	return op2_.map_to_state(state);
  }

  private:
  operand op2_;
};


struct HLT : public instruction
{
  void execute(vm_state& state) override {
	state.PC = state.PS;
  }
};


struct PUSH : public one_parm_instruction
{
  using one_parm_instruction::one_parm_instruction;
  void execute(vm_state& state) override {
	state.RAM[--state.SP] = destination(state);
	++state.PC;
  }
};


struct POP : public one_parm_instruction
{
  using one_parm_instruction::one_parm_instruction;
  void execute(vm_state& state) override {
	destination(state) = state.RAM[state.SP++];
	++state.PC;
  }
};


struct JMP : public one_parm_instruction
{
  using one_parm_instruction::one_parm_instruction;
  void execute(vm_state& state) override {
	state.PC = destination(state);
  }
};


struct MOV : public two_parm_instruction
{
  using two_parm_instruction::two_parm_instruction;
  void execute(vm_state& state) override {
	destination(state) = source(state);
	++state.PC;
  }
};


struct ADD : public two_parm_instruction
{
  using two_parm_instruction::two_parm_instruction;
  void execute(vm_state& state) override {
	destination(state) += source(state);
	++state.PC;
  }
};


struct SUB : public two_parm_instruction
{
  using two_parm_instruction::two_parm_instruction;
  void execute(vm_state& state) override {
	destination(state) -= source(state);
	++state.PC;
  }
};


struct compiler
{
  std::unique_ptr<instruction> create_instruction(std::string i);
  std::unique_ptr<instruction> create_instruction(std::string i, operand op1);
  std::unique_ptr<instruction> create_instruction(std::string i, operand op1, operand op2);
  std::unique_ptr<instruction> make_instruction(std::size_t l, std::vector<std::string>);
};

#endif
