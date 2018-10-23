MODULE_NAME = uxcdps
obj-m   := $(MODULE_NAME).o
$(MODULE_NAME)-objs := read_userspace_file.o

Kernel_path:=/lib/modules/$(shell uname -r)/build  
PWD:=$(shell pwd)
export C_INCLUDE_PATH=/usr/include/i386-linux-gnu/
KBUILD_EXTRA_SYMBOLS += $(PWD)/access_control/Module.symvers 
export KBUILD_EXTRA_SYMBOLS
	
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 
clean:
	rm -rf *.o *.ko *.mod.c *.mod.o Module.*