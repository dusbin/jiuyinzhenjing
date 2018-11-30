#$Id: Makefile,v 1.5 1999/08/13 04:59:31 biboman Exp $

CC        = gcc
BBS_HOME  = ${BBSHOME}
BBSSRC    = ../../src

INCLUDE=-I../../include

CFLAGS   = -O2 -m486 -pipe ${INCLUDE}
# For Solaris, HPUX, IRIX, ... remove above line and use CFLAGS in next line
#CFLAGS   = -O2 -DSYSV ${INCLUDE} -lsocket -lnsl

UTILITY  = fingerd

all: $(UTILITY)

install: $(UTILITY)
	cp $(UTILITY) $(BBSHOME)/bin
	chown root $(BBSHOME)/bin/fingerd
	chmod u+s $(BBSHOME)/bin/fingerd

clean:
	rm -fr *.o *.BAK *.log *~ fingerd

fingerd: fingerd.c
	$(CC) $(CFLAGS) -o fingerd fingerd.c ${BBSSRC}/modetype.c $(BBSSRC)/record.c
