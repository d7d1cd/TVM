#ifndef tvmH
#define tvmH

#include <filesystem>

class toy_virtual_machine
{
  public:
  void compile(std::filesystem::path p);
  void print_state() const;

  private:
  unsigned char R1 = 0, R2 = 0, R3 = 0, R4 = 0;
  unsigned char RAM[256];
};

#endif
