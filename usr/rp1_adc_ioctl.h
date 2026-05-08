#ifndef RP1_ADC_IOCTL_H
#define RP1_ADC_IOCTL_H

#include <linux/types.h>
#include <linux/ioctl.h>

#define RP1_ADC_IOCTL_MAGIC     'k'

struct my_ioctl_data {
    __u32 command;
    __u32 size;
    __u64 large_data;
    __u32 small_data;
};

struct rp1_adc_snapshot {
    __u32 cs;
    __u32 result;
    __u32 div;
    __u32 fcs;
    __u32 fifo;
};

#define IOCTL_GET_ADC_SAMPLE_RATE   _IOR('k', 1, struct my_ioctl_data)
#define IOCTL_GET_ADC_REGISTER      _IOR('k', 2, struct my_ioctl_data)
#define IOCTL_GET_ADC_CS            _IOR('k', 3, struct my_ioctl_data)
#define IOCTL_GET_ADC_SNAPSHOT      _IOR('k', 4, struct rp1_adc_snapshot)

#endif
