#include "pitemp.h"

struct temp_device_data devs[MY_MAX_MINORS];

static int __init_tempdevice(void)
{
    int err, i;

    err = register_chrdev_region(MKDEV(MY_MAJOR, 0), MY_MAX_MINORS, "RP1 Temperature Sensor driver");
    if (err != 0) {
        printk(KERN_INFO "[RP1-IO] Error occurred, could not initialize driver for RP1 temperature sensor.");
        return err;
    }

    for(i = 0; i < MY_MAX_MINORS; i++){
        cdev_init(&devs[i].cdev, &my_fops);
        cdev_add(&devs[i].cdev, MKDEV(MY_MAJOR, i), 1);
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
    struct temp_device_data *my_data;
    
    my_data = container_of(inode->i_cdev, struct temp_device_data, cdev);

    file->private_data = my_data;

    return 1;
}

/* Driver-side response to 'read(int fd, void buf[], size_t cnt)' systemcall when used /dev/temp (?) */
ssize_t my_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset)
{
    struct temp_device_data *my_data;

    my_data = (struct temp_device_data *)file->private_data;
    
    return 1;
}


/* MACRO CALLS */
module_init(__init_tempdevice);
module_exit(__cleanup_tempdevice);

MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
