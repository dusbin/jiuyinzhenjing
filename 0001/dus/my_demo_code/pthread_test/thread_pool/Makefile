#test : main.o thread_pool.o 
#	gcc -g main.o thread_pool.o -lpthread -o test
#mian.o : mian.c thread_pool.h Simplelist.h types.h
#	gcc -c -g main.c
#thread_pool.o : thread_pool.c thread_pool.h Simplelist.h types.h
#	gcc -c -g thread_pool.c
#.PHONY clean :　　　　　　　　　　 
#	rm test main.o thread_pool.o
#
CPPFLAGES=-Iinclude
CFLAGS=-g -Wall
LDFLAGS=-lpthread
CC=gcc
src=$(wildcard *.c)
obj=$(patsubst %.c,%.o,$(src))
target=app
$(target):$(obj)
	$(CC) $^ $(LDFLAGS) -o $@ -lcrypt
	@echo '---------编译成功了-------'
%.o:%.c
	$(CC) -c $< $(CFLAGS) $(CPPFLAGS) -o $@
.PHONY:clean
clean:
	-rm -f *.o
	-rm -f $(target)
