#ifndef PITEMP_H
#define PITEMP_H
/* SPDX-License-Identifier: GPL-2.0 */

#include <linux/module.h>           /* Needed by all modules */
#include <linux/kernel.h>           /* Needed for KERN_INFO */
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/wait.h>

#include <asm/uaccess.h>
#include <asm/ioctl.h>

#define MY_MAJOR            50
#define MY_MAX_MINORS       5
#define DRIVER_AUTHOR       "Cameron Pinchin<cwpinchin@outlook.com>"
#define DRIVER_DESC         "Character driver for the templerature sensor on board the RP1 I/O Controller found on RPi5 devices."
#define DRIVER_LICENSE      "GPL"

extern const struct file_operations my_fops;
extern struct temp_device_data my_data;
extern struct my_ioctl_data mid;

int my_open(struct inode *, struct file *);
int my_release(struct inode *, struct file *);
ssize_t my_read(struct file *file, char *, size_t size, loff_t *offset);
ssize_t my_write(struct file *file, const char *, size_t size, loff_t *offset);
long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

#endif
