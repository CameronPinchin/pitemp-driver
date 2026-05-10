#ifndef RP1_ADC_H
#define RP1_ADC_H
/* SPDX-License-Identifier: GPL-2.0 */

#include <linux/module.h>           /* Needed by all modules */
#include <linux/kernel.h>           /* Needed for KERN_INFO */
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/wait.h>
#include <linux/io.h>

#include <asm/uaccess.h>
#include <asm/ioctl.h>

#define MY_MAJOR                    50
#define MY_MAX_MINORS               5
#define DRIVER_AUTHOR               "Cameron Pinchin<cwpinchin@outlook.com>"
#define DRIVER_DESC                 "Character driver for the templerature sensor on board the RP1 I/O Controller found on RPi5 devices."
#define DRIVER_LICENSE              "GPL"

// this information can be found in the device-tree for the RP1 

/* REGISTER-SPECIFIC INFORMATION */
#define RP1_ADC_BASE_ADDRESS        ((phys_addr_t)0x1f000c8000ULL)  /* 0x400c8000 */

#define RP1_ADC_MAP_SIZE            ((size_t)0x4000) // 16 kB 

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
#define IOCTL_GET_ADC_REGISTER      _IOR('k', 2, struct my_ioctl_data)
#define IOCTL_GET_ADC_CS            _IOR('k', 3, struct my_ioctl_data)
#define IOCTL_GET_ADC_SNAPSHOT      _IOR('k', 4, struct rp1_adc_snapshot)

extern const struct file_operations my_fops;
extern struct temp_device_data my_data;
extern struct my_ioctl_data mid;

int my_open(struct inode *, struct file *);
int my_release(struct inode *, struct file *);
ssize_t my_read(struct file *file, char *, size_t size, loff_t *offset);
ssize_t my_write(struct file *file, const char *, size_t size, loff_t *offset);
long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

#endif
