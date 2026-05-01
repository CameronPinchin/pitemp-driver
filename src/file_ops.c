/* Driver-side response to 'write' systemcall when used /dev/temp (?) 
ssize_t my_write( struct file *file, char __user *user_buffer, size_t size, loff_t *offset )
{
    struct temp_devuce_data *my_data;

    return 1;
}

Driver-side response to 'write' systemcall when used /dev/temp (?) 
static int my_release(struct inode *inode, struct file *file)
{
    struct temp_devuce_data *my_data;

    return 1;
}

*/
