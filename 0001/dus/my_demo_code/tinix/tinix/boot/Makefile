############################################################
#                  Makefile for Tinix                      #
############################################################

ENTRYPOINT      = 0x100000

CC              = gcc
LD              = ld
CFLAGS          = -I ./ -c -fno-builtin 
#-fno-stack-protector
LDFLAGS         = -x -s -Ttext $(ENTRYPOINT) -e start

TINIXKERNEL     = oskernel
KERNEL_BIN      = kernelz
OBJS            = boot.o loader.o console.o

$(TINIXKERNEL) : $(OBJS) $(KERNEL_BIN)
	$(LD) $(LDFLAGS) -o $(TINIXKERNEL).pe $(OBJS)
	objcopy -I pe-i386 -O elf32-i386 $(TINIXKERNEL).pe $(TINIXKERNEL).elf
	gzip $(TINIXKERNEL).elf
$(OBJS) :boot.S loader.c console.c
	$(CC) $(CFLAGS) $^

clean:
	-rm $(OBJS)
	-rm $(KERNEL_BIN)
	-rm $(TINIXKERNEL).*