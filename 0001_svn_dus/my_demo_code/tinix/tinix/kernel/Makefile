############################################################
#                  Makefile for Tinix                      #
############################################################

ENTRYPOINT      = 0xC0000000

CC              = gcc
LD              = ld
CFLAGS          = -I ./ -c -fno-builtin 
#-fno-stack-protector
LDFLAGS         = -s -Ttext $(ENTRYPOINT) -e start -M

KERNEL_BIN      = kernelz
TINIXKERNEL     = kernel.elf
OBJS            = $(INIT) $(MM) $(DEV) $(KERNEL) 
INIT            = head.o main.o
MM              = memory.o
DEV				= console.o
KERNEL 			= vsprintf.o string.o task.o io.o traps.o traps_asm.o

$(KERNEL_BIN) : $(TINIXKERNEL)
	objcopy -S -O binary $(TINIXKERNEL) $(KERNEL_BIN)

$(TINIXKERNEL) : $(OBJS)
	$(LD) $(LDFLAGS) -o $(TINIXKERNEL) $(OBJS) > system.map

$(INIT) :init/head.S init/main.c
	$(CC) $(CFLAGS) $^ 
$(MM) :mm/memory.c
	$(CC) $(CFLAGS) $^
$(DEV) :dev/console.c
	$(CC) $(CFLAGS) $^
$(KERNEL) :kernel/vsprintf.c kernel/string.c kernel/task.c kernel/io.c kernel/traps.c kernel/traps_asm.S
	@$(CC) $(CFLAGS) $^
clean:
	-rm $(OBJS)
	-rm $(TINIXKERNEL)
	-rm $(KERNEL_BIN)
	-rm system.map