#include <iostream>
#include "pin.H"

int count = 0;
int main(int argc, char** argv) {
  if (PIN_Init(argc, argv))
    return -1;

  INS_AddInstrumentFunction([](INS i, void* v) -> void {
    INS_InsertCall(i, IPOINT_BEFORE, (AFUNPTR)[](){++count;}, IARG_END);
  }, 0);

  PIN_AddFiniFunction([](int c, void* v){
    std::cout << count << std::endl;
  }, 0);

  PIN_StartProgram();

  return 0;
}
