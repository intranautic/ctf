#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>


MODULE_LICENSE("GPL");

static void parse_selector(const char* name, uint16_t reg) {
  printk("%s: %hu {\n\tIndex: %d\n\tTI: %d\n\tRPL: %d\n}\n",
    name,
    reg,
    reg >> 3,
    );

  return;
}

static int __init segments_init(void) {
  printk("[segments]: kernel module loaded\n");

  return 0;
}

static void __exit segments_exit(void) {
  printk("[segments]: kernel module unloaded\n");
}

module_init(segments_init);
module_exit(segments_exit);
