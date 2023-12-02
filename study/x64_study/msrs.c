#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include <asm/msr-index.h>


MODULE_LICENSE("GPL");

static int __init msrs_init(void) {
  printk("[msrs]: kernel module loaded\n");
  int64_t efer = __rdmsr(MSR_EFER);
  printk("IA32_EFER = %#llx\n", efer);
  if (efer & EFER_LME)
    printk("IA32_EFER.LME=1\n");
  if (efer & EFER_LMA)
    printk("IA32_EFER.LMA=1\n");
  if (efer & EFER_SVME)
    printk("IA32_EFER.SVME=1\n");

  printk("FS_BASE = %#llx\nGS_BASE = %#llx\n",
    __rdmsr(MSR_FS_BASE),
    __rdmsr(MSR_GS_BASE));

  return 0;
}

static void __exit msrs_exit(void) {
  printk("[msrs]: kernel module unloaded\n");
}

module_init(msrs_init);
module_exit(msrs_exit);
