/* SPDX-License-Identifier: GPL-2.0 */
#include "../include/rp1_adc.h"
#include <asm/uaccess.h>
#include <asm/ioctl.h>
#include <linux/io.h>

struct my_ioctl_data {
    __u32 command;
    __u32 size;
    __u32 data;
};

struct rp1_adc_snapshot {
    __u32 cs;
    __u32 result;
    __u32 div;
    __u32 fcs;
    __u32 fifo;
};

static void fill_ioctl_data_struct(struct my_ioctl_data * data, __u32 cmd, __u32 size, __u32 data)
{
    data->command=cmd;
    data->size=size;
    data->data=data;
}

static __u32 get_sample_rate(__u32 val)
{
    return (48000000)/(96 + val); // TO-DO: double-check this is behaving correctly
}

static int read_register(__u32* val, size_t offset)
{
    if (offset & 0x3)
        return -EINVAL;

    void __iomem *v_addr = ioremap(RP1_ADC_BASE_ADDRESS, RP1_ADC_MAP_SIZE);
    if(!v_addr){
        printk( KERN_ERR "[RP1-ADC] V_addr is NULL, ioremap() failed.\n");
        return -ENOMEM;
    }
    *val = readl(v_addr + offset);

    iounmap(v_addr);

    return 0; // success
}

static int rp1_adc_get_snapshot(struct rp1_adc_snapshot* snap)
{
    void __iomem *v_addr = ioremap(RP1_ADC_BASE_ADDRESS, RP1_ADC_MAP_SIZE);

    if(!v_addr){
        printk( KERN_ERR "[RP1-ADC] V_addr is NULL, ioremap() failed.\n");
        return -ENOMEM;
    }

    snap->cs         = readl(v_addr + RP1_ADC_CS);
    snap->result     = readl(v_addr + RP1_ADC_RESULT);
    snap->div        = readl(v_addr + RP1_ADC_DIV);
    snap->fcs        = readl(v_addr + RP1_ADC_FCS);
    snap->fifo       = readl(v_addr + RP1_ADC_FIFO);

    iounmap(v_addr);

    return 0; // success
}

long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct rp1_adc_data *my_data = (struct rp1_adc_data *)file->private_data;
    struct my_ioctl_data mid;
    struct rp1_adc_snapshot snap;
    __u32 val;
    int ret;

    switch(cmd){
        case IOCTL_GET_ADC_SAMPLE_RATE:
            ret = read_register(&val, RP1_ADC_DIV);
            val = get_sample_rate(val);
            fill_ioctl_data_struct(&mid, cmd, sizeof(val), val);

            if(copy_to_user((struct my_ioctl_data *) arg, &mid, sizeof(mid)) != 0){
                return -EFAULT;
            }

            return 0;
        case IOCTL_GET_ADC_CS:
            ret = read_register(&val, RP1_ADC_CS);
            fill_ioctl_data_struct(&mid, cmd, sizeof(val), val);

            if(copy_to_user((struct my_ioctl_data *) arg, &mid, sizeof(mid)) != 0){
                return -EFAULT;
            }

            return 0;
        case IOCTL_GET_ADC_RESULT:
            ret = read_register(&val, RP1_ADC_RESULT);
            fill_ioctl_data_struct(&mid, cmd, sizeof(val), val);

            if(copy_to_user((struct my_ioctl_data *) arg, &mid, sizeof(mid)) != 0){
                return -EFAULT;
            }

            return 0;
        case IOCTL_GET_ADC_FCS:
            ret = read_register(&val, RP1_ADC_FCS);
            fill_ioctl_data_struct(&mid, cmd, sizeof(val), val);

            if(copy_to_user((struct my_ioctl_data *) arg, &mid, sizeof(mid)) != 0){
                return -EFAULT;
            }

            return 0;
        case IOCTL_GET_ADC_FIFO:
            ret = read_register(&val, RP1_ADC_FIFO);
            fill_ioctl_data_struct(&mid, cmd, sizeof(val), val);

            if(copy_to_user((struct my_ioctl_data *) arg, &mid, sizeof(mid)) != 0){
                return -EFAULT;
            }

            return 0;
        case IOCTL_GET_ADC_SNAPSHOT:
            ret = rp1_adc_get_snapshot(&snap);

            if(copy_to_user((struct rp1_adc_snapshot *) arg, &snap, sizeof(snap)) != 0){
                return -EFAULT;
            }

            return 0;
        default:
            return -ENOTTY;
        
    }

    return 0;
}

