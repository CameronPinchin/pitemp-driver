/* SPDX-License-Identifier: GPL-2.0 */
#include "pitemp.h"

/* ioctl, the ability to perform certain physical dev control tasks.
 *   - the values corresponding to cmd(s) must be chosen.
 *   - lots of ways to do this; pick consecutive numbers starting at 0
 *   - recommended, however, to use the macro: _IOC(dir, type, nr, size)
 *           dir: represents the data transfer (_IOC_NONE, _IOC_READ, _IOC_WRITE)
 *          type: represents the magic number (unique identifier?)
 *            nr: represents the ioctl code for the device
 *          size: represents the size of the transferred data.
 *  - examples of commands: setting buffer size, enabling or disabling the device, get current temperature, get device status, get error codes, etc
 */

struct my_ioctl_data {
    __u32 command;      // command being passed
    __u32 size;         // size of the data being passed
    __u64 data_ptr;     // pointer to the user-space buffer
};

// direction of data transfer relative to the kernel
// i.e., MY_IOCTL_IN: user-space --> kernel-space
//  ADC: Analog-digital-conveter --> addr: 0x400c8000
// adc: bus type: apb, atomic access: y, addr: 0x400c8000

/* IOCTL COMMANDS
 *  CMD_SET_X --> id=1,dir=_IOC_WRITE
 *  CMD_GET_X --> id=2,dir=_IOC_READ
 */

 /*
 struct my_pwm_priv{
    void __iomem *base_addr;
    struct pwm_chip chip;
 }
 */

/* commands. TO-DO: add this to header */
// getters
#define IOCTL_GET_TEMP              _IOC(_IOC_READ, 'k', 1, sizeof(struct my_ioctl_data))
#define IOCTL_GET_SPEED             _IOC(_IOC_READ, 'k', 2, sizeof(struct my_ioctl_data))
#define IOCTL_GET_ADC_REGISTER      _IOC(_IOC_READ, 'k', 3, sizeof(struct my_ioctl_data))
// setters
// fan speed is set with PWM, changing to a desired speed requires reading up on PWM
//  - base the speeds on RPMs? I.e., if user wanted to increase the speed to max RPMs, change the PWM to its maximum (duty == 100)?
//  - how would get this fan information?
#define IOCTL_SET_SPEED _IOC(_IOC_WRITE, 'k', 10, sizeof(struct my_ioctl_data))

long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct temp_device_data *my_data = (struct temp_device_data *)file->private_data;
    struct my_ioctl_data mid;
   
    switch(cmd){
        case IOCTL_GET_TEMP:
            if( copy_to_user(&mid, (struct my_ioctl_data *) arg, sizeof(struct my_ioctl_data)) != 0 ){
                return -EFAULT;
            }

            /* command runs successfully */
            /*  process command here */
            break;
        case IOCTL_GET_SPEED:
            if( copy_to_user(&mid, (struct my_ioctl_data *) arg, sizeof(struct my_ioctl_data)) != 0 ){
                return -EFAULT;
            }

            /* command runs successfully */
            /*  process command here */
            break;
        case IOCTL_GET_ADC_REGISTER:
            printk( KERN_INFO "IOCTL_GET_ADC_REGISTER COMMAND CALLED.\n" );
            if( copy_to_user(&mid, (struct my_ioctl_data *) arg, sizeof(struct my_ioctl_data)) != 0 ){
                return -EFAULT;
            }
            /* THIS WORKS!!!!!! */
            __u32 val;
            // from my understanding, ioremap() allows the kernel access to a memory address by mapping it to a virtual one
            void __iomem *v_addr = ioremap(RP1_ADC_BASE_ADDRESS, RP1_ADC_MAP_SIZE);
            if(!v_addr){
                printk( KERN_INFO "[RP1-ADC] V_addr is NULL, ioremap() failed.\n");
                return -ENOMEM;
            }
            val = readl(v_addr + RP1_ADC_RESULT);
            printk( KERN_INFO "[RP1-IO] DEBUGGING: Value: %d\n", val );

            return 0;
        default:
             /* THIS WORKS!!!!!! */
            // __u32 val;
            // // from my understanding, ioremap() allows the kernel access to a memory address by mapping it to a virtual one
            // void __iomem *v_addr = ioremap(RP1_ADC_BASE_ADDRESS, RP1_ADC_MAP_SIZE);
            // if(!v_addr){
            //     printk( KERN_INFO "[RP1-ADC] V_addr is NULL, ioremap() failed.\n");
            //     return -ENOMEM;
            // }
            // val = readl(v_addr + RP1_ADC_RESULT);
            // printk( KERN_INFO "[RP1-IO] DEBUGGING: Value: %d\n", val );
            return -ENOTTY;
    }

    return 0;
}

