/* SPDX-License-Identifier: GPL-2.0 */
#include "../include/rp1_adc.h"
#include <linux/module.h>           /* Needed by all modules */
#include <linux/kernel.h>           /* Needed for KERN_INFO */
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/types.h>

#define DRIVER_AUTHOR               "Cameron Pinchin<cwpinchin@outlook.com>"
#define DRIVER_DESC                 "Driver for the analog-to-digital converter on the RP1 chip."
#define DRIVER_LICENSE              "GPL"
#define MY_MAJOR                    50
#define MY_MAX_MINORS               5

struct rp1_adc_data {
    struct cdev cdev;
    char buffer[1024]; 
    size_t size;                
};

const struct file_operations my_fops = {
    .owner              = THIS_MODULE,
    .open               = rp1_adc_open,
    .release            = rp1_adc_release,
    .read               = rp1_adc_read,
    .write              = rp1_adc_write,
    .unlocked_ioctl     = rp1_adc_ioctl
};

struct rp1_adc_data devs[MY_MAX_MINORS];

static int rp1_adc_init(void)
{
    int err, i;

    printk(KERN_INFO "[RP1-IO] Initializing %d RP1 ADC devices.\n", MY_MAX_MINORS);

    err = register_chrdev_region(MKDEV(MY_MAJOR, 0), MY_MAX_MINORS, "RP1 ADC Driver");
    if (err != 0) {
        printk(KERN_INFO "[RP1-IO] Error occurred, could not initialize driver for RP1 ADC driver.\n");
        return err;
    }

    for(i = 0; i < MY_MAX_MINORS; i++){
        cdev_init(&devs[i].cdev, &my_fops);
        cdev_add(&devs[i].cdev, MKDEV(MY_MAJOR, i), 1);
    }

    return 0;
}

static void rp1_adc_cleanup(void)
{
    int i;

    printk( KERN_INFO "[RP1-ADC] Cleaning up %d RP1 ADC devices.\n", MY_MAX_MINORS);

    for(i = 0; i < MY_MAX_MINORS; i++) {
        cdev_del(&devs[i].cdev);
    }

    unregister_chrdev_region(MKDEV(MY_MAJOR, 0), MY_MAX_MINORS);
}

int rp1_adc_open(struct inode *inode, struct file *file)
{
    struct rp1_adc_data *my_data;
    int minor = iminor(inode);

    if (minor >= MY_MAX_MINORS) {
        printk( KERN_ERR "[RP1-ADC] Error: Minor %d out of range.\n", minor);
        return -ENODEV;
    }

    my_data = container_of(inode->i_cdev, struct rp1_adc_data, cdev);
    file->private_data = my_data;

    return 0;
}

int rp1_adc_release(struct inode *inode, struct file *file)
{
    struct rp1_adc_data *my_data;
    int minor = iminor(inode);

    if(minor == 1) {
        /* specific action for the final device being closed ? */
    }
    
    return 0;
}

ssize_t rp1_adc_read(struct file *file, char __user *user_buffer, size_t size, loff_t * offset)
{
    struct rp1_adc_data *my_data = (struct rp1_adc_data *)file->private_data;
    ssize_t len = min_t(size_t, my_data->size - *offset, size); // length of the data (?)

    if (len <= 0)
        return 0;

    if (copy_to_user(user_buffer, my_data->buffer + *offset, len) != 0)
        return -EFAULT;

    *offset += len;
    return len;
}

ssize_t rp1_adc_write( struct file *file, const char __user *user_buffer, size_t size, loff_t * offset )
{
    struct rp1_adc_data *my_data = (struct rp1_adc_data *)file->private_data;
    ssize_t len = min_t(size_t, my_data->size - *offset, size);

    if (len <= 0)
        return 0;

    if (copy_from_user(my_data->buffer + *offset, user_buffer, len) != 0)
        return -EFAULT;

    *offset += len;
    return len;
}

/* MACRO CALLS */
module_init(rp1_adc_init);
module_exit(rp1_adc_cleanup);

MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
