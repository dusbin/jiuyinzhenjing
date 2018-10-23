CFLAGS+= -I ../lib_add/inc -I ../lib_sub/inc
libprint.a:print.o
	ar cr libprint.a print.o
print.o:
	gcc -c src/print.c $(CFLAGS)
clean:
	rm libprint.a print.o