CFLAG+= -I .
LFLAG+= -L .
getmachinefinger:getmachinefinger.o
	gcc -o getmachinefinger getmachinefinger.o $(LFLAG) $(CFLAG) -lshcrypt -lssl -lsmbios_c
getmachinefinger.o:
	gcc -c getmachinefinger.c $(CFLAG)
clean:
	rm -rf *.o getmachinefinger