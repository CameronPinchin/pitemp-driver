/* SPDX-License-Identifier: GPL-2.0 */
#ifndef RP1_ADC_H
#define RP1_ADC_H

#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/fs.h>
// ADC block information
#define RP1_ADC_BASE_ADDRESS        ((phys_addr_t)0x1f000c8000ULL) 
#define RP1_ADC_MAP_SIZE            ((size_t)0x4000) 
// register offsets
#define RP1_ADC_CS                  0x00    /* Control and Status register */
#define RP1_ADC_RESULT              0x04    /* Result register */
#define RP1_ADC_FCS                 0x08    /* Frame Check Sequence register */
#define RP1_ADC_FIFO                0x0c    /* First-In-First-Out register (used for temp data) */
#define RP1_ADC_DIV                 0x10    /* Clock Divider register */
// hardware interrupts
#define RP1_ADC_INTR                0x14
#define RP1_ADC_INTE                0x18
#define RP1_ADC_INTF                0x1c
#define RP1_ADC_INTS                0x20
// ioctl commands
#define IOCTL_GET_ADC_SAMPLE_RATE   _IOR('k', 1, struct my_ioctl_data)
#define IOCTL_GET_ADC_CS            _IOR('k', 2, struct my_ioctl_data)
#define IOCTL_GET_ADC_RESULT        _IOR('k', 3, struct my_ioctl_data)
#define IOCTL_GET_ADC_FCS           _IOR('k', 4, struct my_ioctl_data)
#define IOCTL_GET_ADC_FIFO          _IOR('k', 5, struct my_ioctl_data)
#define IOCTL_GET_ADC_SNAPSHOT      _IOR('k', 6, struct rp1_adc_snapshot)

extern const struct file_operations my_fops;
extern struct rp1_adc_data my_data;
extern struct my_ioctl_data mid;

int my_open(struct inode *, struct file *);
int my_release(struct inode *, struct file *);
ssize_t my_read(struct file *file, char *, size_t size, loff_t *offset);
ssize_t my_write(struct file *file, const char *, size_t size, loff_t *offset);
long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

#endif
