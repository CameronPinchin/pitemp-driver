#include <linux/module.h>           /* Needed by all modules */
#include <linux/kernel.h>           /* Needed for KERN_INFO */
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <asm/uaccess.h>

#define MY_MAJOR            50
#define MY_MAX_MINORS       5
#define DRIVER_AUTHOR       "Cameron Pinchin<cwpinchin@outlook.com>"
#define DRIVER_DESC         "Character driver for the templerature sensor on board the RP1 I/O Controller found on RPi5 devices."
#define DRIVER_LICENSE      "MIT"

static int my_open(struct inode *, struct file *);
ssize_t my_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset);
ssize_t my_write( struct file *file, char __user *user_buffer, size_t size, loff_t *offset );

struct temp_device_data {
    struct cdev cdev;
    char buffer[1024];          // unsure what to size
    int16_t size;           // likely change, provides 16-bits though
};

struct temp_device_data devs[MY_MAX_MINORS];

const struct file_operations my_fops = {
    .owner  = THIS_MODULE,
    .open   = my_open,
    .read   = my_read
};