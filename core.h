#ifndef coreH
#define coreH

#include <array>


struct vm_state
{
  std::size_t PC = 0;
  std::size_t PS = 0;
  std::array<char, 4> R = {};
  std::array<char, 256> RAM = {};
};


struct instruction
{
  virtual void execute(vm_state&) = 0;
  virtual ~instruction() {}
};


struct operand
{
  enum { REGISTER, ADDRESS, CONSTANT } type;
  char value;
  char& map_to_state(vm_state& state) {
	switch (type) {
	  case REGISTER: return state.R[value];
	  case ADDRESS:  return state.RAM[value];
	  case CONSTANT: return value;
	}
  }
};


class one_parm_instruction : public instruction
{
  public:
  one_parm_instruction(operand op1)
  : op1_{op1} {}

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

  protected:
  char source(vm_state& state) {
	return op2_.map_to_state(state);
  }

  private:
  operand op2_;
};


struct MOV : public two_parm_instruction
{
  MOV(operand op1, operand op2)
  : two_parm_instruction(op1, op2) {}

  void execute(vm_state& state) override {
	destination(state) = source(state);
	++state.PC;
  }
};


struct ADD : public two_parm_instruction
{
  ADD(operand op1, operand op2)
  : two_parm_instruction(op1, op2) {}

  void execute(vm_state& state) override {
	destination(state) += source(state);
	++state.PC;
  }
};


#endif
