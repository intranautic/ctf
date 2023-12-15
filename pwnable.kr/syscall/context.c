// store user context for kernel iretq return to userland
typedef struct context context_t;
struct context {
  size_t cs;
  size_t ss;
  size_t sp;
  size_t eflags;
};

static context_t g_context;

// asm (inst : out : in : clobr)
void context_store(void) {
  asm volatile (
    ".intel_syntax noprefix;"
    "mov %[a], cs;"
    "mov %[b], ss;"
    "mov %[c], esp;"
    "pushf;"
    "pop %[d];"
    ".att_syntax;"
    
