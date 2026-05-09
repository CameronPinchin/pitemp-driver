# SPDX-License-Identifier: GPL-2.0
# Differs from usual format
# Calls on the kernels build system instead of directly using gcc
KERNEL_VERSION:=$(shell uname -r)

# ccflags-y --> adds compiler flags

obj-m += rp1_adc.o
rp1_adc-objs := rp1_adc_init.o rp1_adc_ioctl.o

all:
	make -C /lib/modules/$(KERNEL_VERSION)/build M=$(PWD) modules
	@echo "Building module for kernel version $(KERNEL_VERSION)"

clean:
	make -C /lib/modules/$(KERNEL_VERSION)/build M=$(PWD) clean
	@echo "Cleaning module for kernel version $(KERNEL_VERSION)"
