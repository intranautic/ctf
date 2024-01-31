// smp call function on all cores, useful for setting cpu bits on smp system
// https://lwn.net/Articles/271039/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/smp.h>

MODULE_LICENSE("GPL");

static void smp_callme(void* info) {
  printk("[smpcall]: core #%d\n",
    smp_processor_id());
  return;
}

static int __init smpcall_init(void) {
  printk("[smpcall]: module loaded on core #%d\n",
    smp_processor_id());
  smp_call_function(smp_callme, NULL, 0);
  return 0;
}

static void __exit smpcall_exit(void) {
  printk("[smpcall]: module unloaded\n");
  return;
}

module_init(smpcall_init);
module_exit(smpcall_exit);
