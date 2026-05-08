/* SPDX-License-Identifier: GPL-2.0 */
#include "pitemp.h"

struct temp_device_data {
    struct cdev cdev;
    char buffer[1024];          // unsure what to size
    size_t size;                // likely change, provides 16-bits though
};

const struct file_operations my_fops = {
    .owner              = THIS_MODULE,
    .open               = my_open,
    .release            = my_release,
    .read               = my_read,
    .write              = my_write,
    .unlocked_ioctl     = my_ioctl
};

struct temp_device_data devs[MY_MAX_MINORS];

static int init_temperature_device(void)
{
    int err, i;

    printk(KERN_INFO "[RP1-IO] Initializing %d RP1-IO temperature sensor devices.\n", MY_MAX_MINORS);

    err = register_chrdev_region(MKDEV(MY_MAJOR, 0), MY_MAX_MINORS, "RP1 Temperature Sensor driver");
    if (err != 0) {
        printk(KERN_INFO "[RP1-IO] Error occurred, could not initialize driver for RP1 temperature sensor.\n");
        return err;
    }

    for(i = 0; i < MY_MAX_MINORS; i++){
        cdev_init(&devs[i].cdev, &my_fops);
        cdev_add(&devs[i].cdev, MKDEV(MY_MAJOR, i), 1);
    }

    return 0;
}

static void cleanup_temperature_device(void)
{
    int i;

    printk( KERN_INFO "[RP1-IO] Cleaning up %d RP1-IO temperature sensor devices.\n", MY_MAX_MINORS);

    for(i = 0; i < MY_MAX_MINORS; i++) {
        cdev_del(&devs[i].cdev);
    }

    unregister_chrdev_region(MKDEV(MY_MAJOR, 0), MY_MAX_MINORS);
}

/* Driver-side response to 'open' systemcall when used /dev/temp (?) */
int my_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "[RP1-IO] Debugging: my_open() called.\n");
    struct temp_device_data *my_data;
    int minor = iminor(inode);

    if (minor >= MY_MAX_MINORS) {
        printk( KERN_ERR "[RP1-ADC] Error: Minor %d out of range.\n", minor);
        return -ENODEV;
    }

    my_data = container_of(inode->i_cdev, struct temp_device_data, cdev);
    file->private_data = my_data;

    // initialize device
    printk(KERN_INFO "[RP1-IO] Minor for device is: %d\n", minor); 

    return 0; // success status 
}

int my_release(struct inode *inode, struct file *file)
{
    return 0;
}

/* Driver-side response to 'read(int fd, void buf[], size_t cnt)' systemcall when used /dev/temp (?) */
ssize_t my_read(struct file *file, char __user *user_buffer, size_t size, loff_t * offset)
{
    struct temp_device_data *my_data = (struct temp_device_data *)file->private_data;
    ssize_t len = min_t(size_t, my_data->size - *offset, size); // length of the data (?)

    if (len <= 0)
        return 0;

    // reads data from my_data->buffer to the user_buffer
    if (copy_to_user(user_buffer, my_data->buffer + *offset, len) != 0)
        return -EFAULT;

    *offset += len;
    return len;
}

ssize_t my_write( struct file *file, const char __user *user_buffer, size_t size, loff_t * offset )
{
    struct temp_device_data *my_data = (struct temp_device_data *)file->private_data;
    ssize_t len = min_t(size_t, my_data->size - *offset, size);

    if (len <= 0)
        return 0;

    if (copy_from_user(my_data->buffer + *offset, user_buffer, len) != 0)
        return -EFAULT;

    *offset += len;
    return len;
}


/* MACRO CALLS */
module_init(init_temperature_device);
module_exit(cleanup_temperature_device);

MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
