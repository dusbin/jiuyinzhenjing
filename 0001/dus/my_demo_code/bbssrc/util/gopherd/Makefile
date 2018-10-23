#$Id: Makefile,v 1.4 1999/08/16 03:18:49 biboman Exp $

CC        = gcc
BBSSRC    = ../../src
INCLUDE   = -I../../include

mCFLAGS   = $(CFLAGS) ${INCLUDE}

all: gopherd

install: gopherd
	cp gopherd $(BBSHOME)/bin
	chown root:bbs $(BBSHOME)/bin/gopherd
	chmod u+s $(BBSHOME)/bin/gopherd

clean:
	rm -fr *.o *.BAK *.log *~ gopherd

gopherd: gopherd.c
	$(CC) $(mCFLAGS) -o gopherd gopherd.c
