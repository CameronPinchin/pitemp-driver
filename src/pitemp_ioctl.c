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
    __u32 command;          // command being passed
    __u32 size;             // size of the data being passed
    __u64 large_data;   // pointer to the user-space buffer
    __u32 small_data;   //  unsure if this is a good idea to split it up like this
};

struct rp1_adc_snapshot {
    __u32 cs;
    __u32 result;
    __u32 div;
    __u32 fcs;
    __u32 fifo;
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

#define IOCTL_GET_ADC_SAMPLE_RATE   _IOR('k', 1, struct my_ioctl_data)
#define IOCTL_GET_ADC_REGISTER      _IOR('k', 2, struct my_ioctl_data)
#define IOCTL_GET_ADC_CS            _IOR('k', 3, struct my_ioctl_data)
#define IOCTL_GET_ADC_SNAPSHOT      _IOR('k', 4, struct rp1_adc_snapshot)

// setters
// fan speed is set with PWM, changing to a desired speed requires reading up on PWM
//  - base the speeds on RPMs? I.e., if user wanted to increase the speed to max RPMs, change the PWM to its maximum (duty == 100)?
//  - how would get this fan information?
#define IOCTL_SET_SPEED _IOW('k', 10, sizeof(struct my_ioctl_data))

static void fill_ioctl_data_struct(struct my_ioctl_data * data, __u32 cmd, __u32 size, __u64 large_data, __u32 small_data)
{
    data->command=cmd;
    data->size=size;
    data->large_data=large_data;
    data->small_data=small_data;
}

static __u32 get_sample_rate(__u32 val)
{
    return (48000000)/(96 + val);
}

static int read_register(__u32* val, size_t offset)
{
    if (offset & 0x3)
        return -EINVAL;

    void __iomem *v_addr = ioremap(RP1_ADC_BASE_ADDRESS, RP1_ADC_MAP_SIZE);
    if(!v_addr){
        printk( KERN_ERR "[RP1-ADC] V_addr is NULL, ioremap() failed.\n");
    }
    *val = readl(v_addr + offset);

    iounmap(v_addr);

    return 0;
}

long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct temp_device_data *my_data = (struct temp_device_data *)file->private_data;
    struct my_ioctl_data mid;
    __u32 val;
    switch(cmd){
        case IOCTL_GET_ADC_SAMPLE_RATE:
            read_register(&val, RP1_ADC_DIV);
            val = get_sample_rate(val);
            fill_ioctl_data_struct(&mid, cmd, sizeof(val), -1, val);

            if( copy_to_user((struct my_ioctl_data *) arg, &mid, sizeof(mid)) != 0 ){
                return -EFAULT;
            }

            return 0;
        case IOCTL_GET_ADC_CS:
            read_register(&val, RP1_ADC_CS);
            fill_ioctl_data_struct(&mid, cmd, sizeof(val), -1, val);

            if( copy_to_user((struct my_ioctl_data *) arg, &mid, sizeof(mid)) != 0 ){
                return -EFAULT;
            }

            return 0;
        case IOCTL_GET_ADC_SNAPSHOT:
            struct rp1_adc_snapshot snap;
            void __iomem *v_addr = ioremap(RP1_ADC_BASE_ADDRESS, RP1_ADC_MAP_SIZE);

            if(!v_addr){
                printk( KERN_ERR "[RP1-ADC] V_addr is NULL, ioremap() failed.\n");
                return -ENOMEM;
            }

            snap.cs         = readl(v_addr + RP1_ADC_CS);
            snap.result     = readl(v_addr + RP1_ADC_RESULT);
            snap.div        = readl(v_addr + RP1_ADC_DIV);
            snap.fcs        = readl(v_addr + RP1_ADC_FCS);
            snap.fifo       = readl(v_addr + RP1_ADC_FIFO);

            if( copy_to_user((struct rp1_adc_snapshot *) arg, &snap, sizeof(snap)) != 0 ){
                iounmap(v_addr);
                return -EFAULT;
            }
            iounmap(v_addr);
            return 0;
        default:
            /* TO-DO: 
             *  - Should the default case give more information?
             */
            return -ENOTTY;
        
    }

    return 0;
}

