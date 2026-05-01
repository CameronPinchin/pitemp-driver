#include "pitemp.h"

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

/* MACRO CALLS */
module_init(__init_tempdevice);
module_exit(__cleanup_tempdevice);

MODULE_LICENSE("MIT");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
