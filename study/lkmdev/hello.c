#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/slab.h> 

MODULE_LICENSE("GPL");

void* buffer = 0;

static int hello_init(void) {
  buffer = kmalloc(1, GFP_KERNEL); 
  printk("Allocation size @ %#lx: %d\n", buffer, ksize(buffer));
  return 0;
}

static void hello_exit(void) {
  kfree(buffer);
  printk("Exit!!");
  return;
}

module_init(hello_init);
module_exit(hello_exit);
