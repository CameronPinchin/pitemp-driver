#include "pitemp.h"
/* ioctl, the ability to perform certain physical dev control tasks.
 *   - the values corresponding to cmd(s) must be chosen.
 *   - lots of ways to do this; pick consecutive numbers starting at 0
 *   - recommended, however, to use the macro: _IOC(dir, type, nr, size)
 *           dir: represents the data transfer (_IOC_NONE, _IOC_READ, _IOC_WRITE)
 *          type: represents the magic number (unique identifier?)
 *            nr: represents the ioctl code for the device
 *          size: represents the size of the transferred data.
 */

long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    // temp_device_data *my_data = (struct temp_device_data *)file->private_data;
    // struct ---temp_ioctl_data mid;

    return 0;
}

