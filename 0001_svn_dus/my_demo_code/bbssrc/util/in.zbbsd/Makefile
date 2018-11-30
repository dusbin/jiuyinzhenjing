# ------------------------------------------------------ #
#  Makefile for in.zbbsd                                 #
#  $Id: Makefile,v 1.7 1999/07/25 13:44:55 edwardc Exp $
# ------------------------------------------------------ #

# ------------------------------------------------------ #
# NetBSD, 386BSD, FreeBSD 
# ------------------------------------------------------
OS_DEF   = -DNETBSD -DBSD44 -DTWO_LONGJMP_ARGS -DHAVE_UTMP
CC       = gcc 
CFLAGS    = -O2
LIBS     = -ltermcap -lcrypt -lcompat
LIBSYBBSD = -lkvm -lutil -lcrypt


# ------------------------------------------------------
# Linux, if yor're using RedHat, add -DREDHAT in OS_DEF
# ------------------------------------------------------
#OS_DEF   = -DLINUX -DTWO_LONGJMP_ARGS -DHAVE_UTMP -DUSE_SHADOW
#CC       = gcc 
#CFLAGS    = -O6 -s -pipe -fomit-frame-pointer -I/usr/include/bsd
#LIBS     = -ltermcap -lbsd


# ------------------------------------------------------
# SunOS 4.1.x
# ------------------------------------------------------
#OS_DEF	= -DSunOS -DTWO_LONGJMP_ARGS -DHAVE_UTMP
#CC	= gcc
#CFLAGS	= -O6 -s -pipe -fomit-frame-pointer -fstrength-reduce
#LIBS	= -ltermcap 
#LIBSYBBSD = -lkvm

# ------------------------------------------------------
# OSF/1
# ------------------------------------------------------
#OS_DEF	= -DOSF1 -DTWO_LONGJMP_ARGS -DHAVE_UTMP
#3CC	= gcc
#CFLAGS	= -O6 -s -pipe -fomit-frame-pointer -fstrength-reduce
#LIBS	= -ltermcap 


# ------------------------------------------------------ #
# 下列的 make rules 不需修改				 #
# ------------------------------------------------------ #

ALL =	in.zbbsd

ETC =	Makefile $(HDR)

.SUFFIXES: .o .c .ln

.c.o:   ;   $(CC) $(CFLAGS) $(OS_DEF) -c $*.c
.c.ln:  ;   lint -abhi $*.c

all: $(ALL)

lint: $(LNFILES)
	lint -abh $(LNFILES)

in.zbbsd: in.zbbsd.c
	$(CC) $(CFLAGS) -o in.zbbsd -I../../include $(OS_DEF) in.zbbsd.c $(LIBSYBBSD) $(LIB)

clean:
	rm -fr *~ in.zbbsd DEAD* *BAK
