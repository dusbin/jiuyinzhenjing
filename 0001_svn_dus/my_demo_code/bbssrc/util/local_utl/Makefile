# Generated automatically from Makefile.in by configure.
#  Makefile for local utility tool kit
# 
#  $Id: Makefile.in,v 1.2 1999/07/16 01:29:22 edwardc Exp $
#

CC       = gcc
CFLAGS   = -O -Wunused -I../include -I../../include
BBSHOME	 = /home/bbs

UTILITY  = account averun bbstop bbspop3d bfinger birthday horoscope \
		   http-finger newacct poststat usage showuser

.c.o:   ;   $(CC) $(CFLAGS) -c $*.c

all: $(UTILITY)

install: $(UTILITY)
	cp $(UTILITY) $(BBSHOME)/bin

clean:
	rm -fr *.o *.BAK *.log $(UTILITY) *core* DEAD* *~

bbspop3d:
	$(CC) $(CFLAGS) -o bbspop3d bbspop3d.c -lcrypt

bfinger: bfinger.c ../../src/modetype.c 
	$(CC) $(CFLAGS) -o bfinger bfinger.c ../../src/modetype.c

http-finger: http-finger.c ../../src/modetype.c 
	$(CC) $(CFLAGS) -o http-finger http-finger.c ../../src/modetype.c

usage: usage.c ../../src/record.c
	$(CC) $(CFLAGS) -o usage usage.c ../../src/record.c
