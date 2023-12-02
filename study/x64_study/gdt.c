#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>


MODULE_LICENSE("GPL");

static int __init gdt_init(void) {
  printk("[gdt]: kernel module loaded\n");

  return 0;
}

static void __exit gdt_exit(void) {
  printk("[gdt]: kernel module unloaded\n");
}

module_init(gdt_init);
module_exit(gdt_exit);
