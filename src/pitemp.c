#include <linux/module.h>           /* Needed by all modules */
#include <linux/kernel.h>           /* Needed for KERN_INFO */
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <asm/uaccess.h>

#define DRIVER_AUTHOR           "Cameron Pinchin <cwpinchin@outlook.com>"
#define DRIVER_DESC             "Driver for temperature sensor on the RP1 I/O Controller"

#define MY_MAJOR                50  /* Maybe change */
#define MY_MAX_MINORS           5   /* Maybe change */

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
    .write  = my_write
};

static int __init_tempdevice(void)
{
    int err, i;

    err = register_chrdev_region(MKDEV(MY_MAJOR, 0), MY_MAX_MINORS, "RP1 Temperature Sensor driver");
    if (err != 0) {
        printk(KERN_INFO "[RP1-IO] Error occurred, could not initialize driver for RP1 temperature sensor.");
        return err;
    }

    for(i = 0; i < MY_MAX_MINORS; i++){
        cdev_init( &devs[i].cdev, &my_fops );
        cdev_add( &devs[i].cdev, MKDEV(MY_MAJOR, i), 1);
    }

    printk(KERN_INFO "[RP1-IO] Initialization of %d RP1-IO temperature sensor devices was successful.", MY_MAX_MINORS);

    return 0;
}

static void __cleanup_tempdevice(void)
{
    int i;

    for(i = 0; i < MY_MAX_MINORS; i++) {
        cdev_del(&devs[i].cdev);
    }

    unregister_chrdev_region(MKDEV(MY_MAJOR, 0), MY_MAX_MINORS);
    printk( KERN_INFO "[RP1-IO] Cleanup of %d RP1-IO temperature sensor devices was successful.", MY_MAX_MINORS);
}

/* Driver-side response to 'open' systemcall when used /dev/temp (?) */
static int my_open(struct inode *inode, struct file *file)
{

}

/* Driver-side response to 'read' systemcall when used /dev/temp (?) */
static int my_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset)
{

}

/* Driver-side response to 'write' systemcall when used /dev/temp (?) */
static int my_write( struct file *file, char __user *user_buffer, size_t size, loff_t *offset )
{

}

/* MACRO CALLS */
module_init(__init_tempdevice);
module_exit(__cleanup_tempdevice);

MODULE_LICENSE("MIT");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);

