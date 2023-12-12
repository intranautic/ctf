#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include <asm/msr-index.h>

#include "debug.h"

MODULE_LICENSE("GPL");

static int __init msrs_init(void) {
  dbg_print("kernel module loaded");
  int64_t efer = __rdmsr(MSR_EFER);
  dbg_print("IA32_EFER = %#llx", efer);
  if (efer & EFER_LME)
    dbg_print("IA32_EFER.LME=1");
  if (efer & EFER_LMA)
    dbg_print("IA32_EFER.LMA=1");
  if (efer & EFER_SVME)
    dbg_print("IA32_EFER.SVME=1");

  dbg_print("FS_BASE = %#llx", __rdmsr(MSR_FS_BASE));
  dbg_print("GS_BASE = %#llx", __rdmsr(MSR_GS_BASE));
  return 0;
}

static void __exit msrs_exit(void) {
  dbg_print("kernel module unloaded");
  return;
}

module_init(msrs_init);
module_exit(msrs_exit);
