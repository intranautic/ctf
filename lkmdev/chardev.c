#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/cdev.h>

#include "debug.h"

MODULE_LICENSE("GPL");

static int __init chardev_init(void) {
  dbg_print("chardev kernel module loaded");
  return 0;
}

static void __exit chardev_exit(void) {
  dbg_print("chardev kernel module unloaded");
  return;
}

module_init(chardev_init);
module_exit(chardev_exit);
