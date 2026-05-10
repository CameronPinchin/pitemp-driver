#ifndef RP1_ADC_IOCTL_H
#define RP1_ADC_IOCTL_H

#include <linux/types.h>
#include <linux/ioctl.h>

#define RP1_ADC_IOCTL_MAGIC         'k'

struct my_ioctl_data {
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

#define IOCTL_GET_ADC_SAMPLE_RATE   _IOR(RP1_ADC_IOCTL_MAGIC, 1, struct my_ioctl_data)
#define IOCTL_GET_ADC_CS            _IOR(RP1_ADC_IOCTL_MAGIC, 2, struct my_ioctl_data)
#define IOCTL_GET_ADC_RESULT        _IOR(RP1_ADC_IOCTL_MAGIC, 3, struct my_ioctl_data)
#define IOCTL_GET_ADC_FCS           _IOR(RP1_ADC_IOCTL_MAGIC, 4, struct my_ioctl_data)
#define IOCTL_GET_ADC_FIFO          _IOR(RP1_ADC_IOCTL_MAGIC, 5, struct my_ioctl_data)
#define IOCTL_GET_ADC_SNAPSHOT      _IOR(RP1_ADC_IOCTL_MAGIC, 6, struct rp1_adc_snapshot)

#endif
