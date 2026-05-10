# SPDX-License-Identifier: GPL-2.0
KERNEL_VERSION:=$(shell uname -r)

obj-m += rp1_adc_custom.o
rp1_adc_custom-objs := rp1_adc_init.o rp1_adc_ioctl.o

all:
	make -C /lib/modules/$(KERNEL_VERSION)/build M=$(PWD) modules
	@echo "Building module for kernel version $(KERNEL_VERSION)"

clean:
	make -C /lib/modules/$(KERNEL_VERSION)/build M=$(PWD) clean
	@echo "Cleaning module for kernel version $(KERNEL_VERSION)"
