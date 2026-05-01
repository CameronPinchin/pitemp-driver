#include <linux/module.h>           /* Needed by all modules */
#include <linux/kernel.h>           /* Needed for KERN_INFO */
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <asm/uaccess.h>

struct temp_device_data {
    struct cdev cdev;
    char buffer[1024];          // unsure what to size
    int16_t size;           // likely change, provides 16-bits though
}

struct temp_device_data devs[MY_MAX_MINORS];

const struct file_operations my_fops = {
    .owner  = THIS_MODULE,
    .open   = my_open,
    .read   = my_read,
    .write  = my_write,
    .release = my_release,
    .unlocked_ioctl = my_ioctl
};