# Differs from usual format
# Calls on the kernels build system instead of directly using gcc
KERNEL_VERSION:=$(shell uname -r)

obj-m += chardriver.o
pitemp-objs := init.o file_ops.o ioctl.o 

all:
	make -C /lib/modules/$(KERNEL_VERSION)/build M=$(PWD) modules
	@echo "Building module for kernel version $(KERNEL_VERSION)"

clean:
	make -C /lib/modules/$(KERNEL_VERSION)/build M=$(PWD) clean
	@echo "Cleaning module for kernel version $(KERNEL_VERSION)"
