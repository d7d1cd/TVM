#ifndef tvmH
#define tvmH

#include "core.h"
#include <filesystem>
#include <vector>


class toy_virtual_machine
{
  public:
  void compile(std::filesystem::path p);
  void print_state() const;
  void run();

  private:
  vm_state state_;
  std::vector<std::unique_ptr<instruction>> rom_;
};


#endif
