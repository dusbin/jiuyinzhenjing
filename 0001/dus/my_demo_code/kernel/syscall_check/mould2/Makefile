MODULE_NAME:=mould2
obj-m	:= $(MODULE_NAME).o
PWD:=$(shell pwd)
objs := mould2.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	rm -fr .*.o  .*.ko.*  .*.mod.o .*.mod.c Module.* *.ko *.unsigned *.o *.order *.mod.c .$(MODULE_NAME)* .tmp*
