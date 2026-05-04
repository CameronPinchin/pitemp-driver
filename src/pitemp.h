#ifndef PITEMP_H
#define PITEMP_H

#include <linux/module.h>           /* Needed by all modules */
#include <linux/kernel.h>           /* Needed for KERN_INFO */
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <asm/uaccess.h>
#include <asm/ioctl.h>

#define MY_MAJOR            50
#define MY_MAX_MINORS       5
#define DRIVER_AUTHOR       "Cameron Pinchin<cwpinchin@outlook.com>"
#define DRIVER_DESC         "Character driver for the templerature sensor on board the RP1 I/O Controller found on RPi5 devices."
#define DRIVER_LICENSE      "MIT"

#define MY_IOCTL_IN _IOC(_IOC_WRITE, 'k', 1, sizeof(my_ioctl_data))
// #define MY_IOCTL_OUT _IOC(_IOC_READ, 'k', x, y )
extern const struct file_operations my_fops;

static int init_tempdevice(void);
static void cleanup_tempdevice(void);
static int my_open(struct inode *, struct file *);
ssize_t my_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset);
ssize_t my_write(struct file *file, char __user *user_buffer, size_t size, loff_t *offset);
static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

#endif
