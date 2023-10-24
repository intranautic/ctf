// playing with emulation
#include <unicorn/unicorn.h>

#define ERROR_LOG(msg, err) \
  { \
    printf(msg, err); \
    return -1; \
  }

#define MAP_BASE 0x00001000
#define MAP_SIZE (0x00002000 << 8)
#define STACK_BASE 
#define STACK_SIZE

typedef struct cpu_state cpu_state;
struct cpu_state {
  long reg_rax;
  long reg_rdi;
  long reg_rsi;
  long reg_rdx
  long reg_rbx;
  long reg_rcx;
  long reg_rsp;
  long reg_rbp;
  long reg_r8;
  long reg_r9;
  long reg_r10;
  long reg_r11;
  long reg_r12;
  long reg_r13;
  long reg_r14;
  long reg_r15;
};

int main(int argc, char** argv) {
  // init handler
  uc_engine* uc;
  uc_err err = uc_open(UC_ARCH_X86, UC_MODE_64, &uc);
  if (err != UC_ERR_OK) 
    ERROR_LOG("Failed on uc_open() with error: %u\n", err);
  // map memory, stack & initialize registers
  cpu_state cpu = {0};
  uc_mem_map(uc, MAP_BASE, MAP_SIZE, UC_PROT_ALL);

  

  return 0;
}
