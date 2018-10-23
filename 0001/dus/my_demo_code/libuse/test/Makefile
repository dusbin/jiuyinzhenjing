CFLAGS+= -I ../libprint/inc
LDFLAGS+= -L ../libprint -lprint
CFLAGS+= -I ../lib_add/inc -I ../lib_sub/inc
LDFLAGS+= -L ../lib_add -ladd -L ../lib_sub -lsub
test:test.o
	gcc -o test test.o $(CFLAGS) $(LDFLAGS)
test.o:
	gcc -c test.c $(CFLAGS)
clean:
	rm test test.o
	
	