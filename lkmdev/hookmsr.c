// x86_64 uses MSR_LSTAR for syscall/sysret instructions
// hooking this msr will allow us to redirect to our own fraudulent sys_call_table
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#define MSR_LSTAR 0xc0000082

MODULE_LICENSE("GPL");



static int __init hookmsr_init(void) {
  return 0;
}
static void __exit hookmsr_exit(void) {
  return;
}

module_init(hookmsr_init);
module_exit(hookmsr_exit);
