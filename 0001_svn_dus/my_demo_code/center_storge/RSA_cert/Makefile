INCFLAG+= -I include/openssl/ -I rsa/inc/ -I req/inc/
CFLAG+= -L lib/
rsatool:rsatool.o rsa.o req.o
	gcc -o rsatool rsatool.o rsa.o req.o $(CFLAG) -lcomm -ldl
rsatool.o:
	gcc -c rsatool.c $(INCFLAG)
rsa.o:
	gcc -c rsa/src/rsa.c $(INCFLAG) 
req.o:
	gcc -c req/src/req.c $(INCFLAG)
clean:
	rm -rf rsatool rsatool.o rsa.o req.o