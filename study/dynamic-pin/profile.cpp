// profile execution of process
#include <stdio.h>
#include "pin.H"

KNOB<std::string> log_file(KNOB_MODE_WRITEONCE, "pintool", "o", "count.out", "output file name");

unsigned long count = 0;


int main(int argc, char** argv) {
  PIN_InitSymbols();
  if (PIN_Init(argc, argv)) {
    fprintf(stderr, "This tool profiles the binary\n%s",
      KNOB_BASE::StringKnobSummary().c_str());
    return -1;
  }

  PIN_SetSyntaxIntel();

  INS_AddInstrumentFunction([](INS i, void* v) -> void {
    // instrument a call before every instruction via IPOINT_BEFORE
    INS_InsertCall(i, IPOINT_BEFORE, (AFUNPTR)[](){++count;}, IARG_END);
  }, 0);

  PIN_AddFiniFunction([](int c, void* v){
    fprintf(fopen(log_file.Value().c_str(), "w"),
      "Instruction Count: %lu\n", count);
  }, 0);

  PIN_StartProgram();
  return 0;
}
