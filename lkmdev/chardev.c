/*
 * basic character device which allows the user to read/write to and from a
 * buffer held by the module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/fs.h>

#include "debug.h"

#define DEVICE_NAME "chardev"

MODULE_LICENSE("GPL");

static int chardev_open(struct inode*, struct file*);
static int chardev_release(struct inode*, struct file*);
static ssize_t chardev_read(struct file*, char __user*, size_t, loff_t*);
static ssize_t chardev_write(struct file*, const char __user*, size_t, loff_t*);

/* operations associated with open file: struct file */
static struct file_operations chardev_fops = {
  .owner = THIS_MODULE,
  .open = chardev_open,
  .release = chardev_release,
  .read = chardev_read,
  .write = chardev_write
};

typedef struct state state_t;
struct state {
  char g_buffer[500];
  dev_t devno;
  struct cdev chardev;
};

static state_t g_state;

static int __init chardev_init(void) {
  dbg_info("chardev kernel module loaded");
  // dynamically allocate device number & region
  if (alloc_chrdev_region(&g_state.devno, 0, 1, DEVICE_NAME) < 0)
    dbg_error("could not dynamically allocate device region");
  dbg_info("device major/minor no: %d %d",
    MAJOR(g_state.devno), MINOR(g_state.devno));

  dbg_info("initializing and adding chardev with fileops");
  cdev_init(&g_state.chardev, &chardev_fops);
  g_state.chardev.owner = THIS_MODULE;
  g_state.chardev.ops = &chardev_fops;
  if (cdev_add(&g_state.chardev, g_state.devno, 1) < 0)
    dbg_error("could not add character device");

  dbg_info("device allocated and registered");
  return 0;
}

static void __exit chardev_exit(void) {
  dbg_info("unregistering device region and device");
  unregister_chrdev_region(g_state.devno, 1);
  cdev_del(&g_state.chardev);
  dbg_info("chardev kernel module unloaded");
  return;
}

static int chardev_open(struct inode* inode, struct file* file) {
  return 0;
}

static int chardev_release(struct inode* inode, struct file* file) {
  return 0;
}

static ssize_t chardev_read(struct file* file,
    char __user* buffer,
    size_t len,
    loff_t* off) {
  return 0; 
}

static ssize_t chardev_write(struct file* file,
    const char __user* buffer,
    size_t len,
    loff_t* off) {
  return 0;
}

module_init(chardev_init);
module_exit(chardev_exit);
