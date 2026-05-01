#include "pitemp.h"

/* Driver-side response to 'open' systemcall when used /dev/temp (?) */
static int my_open(struct inode *inode, struct file *file)
{
    struct temp_device_data *my_data;
    
    my_data = container_of(inode->i_cdev);

    file->private_data = my_data;
}

/* Driver-side response to 'read(int fd, void buf[], size_t cnt)' systemcall when used /dev/temp (?) */
static int my_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset)
{
    struct temp_device_data *my_data;

    my_data = (struct temp_device_data *)file->private_data;
}

/* Driver-side response to 'write' systemcall when used /dev/temp (?) */
static int my_write( struct file *file, char __user *user_buffer, size_t size, loff_t *offset )
{
    struct temp_devuce_data *my_data;
    
}

/* Driver-side response to 'write' systemcall when used /dev/temp (?) */
static int my_release(struct inode *inode, struct file *file)
{
    struct temp_devuce_data *my_data;

}

