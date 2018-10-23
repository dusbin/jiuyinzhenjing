#############################################################
####   Setup the following site-specific information     ####
#############################################################
# $Id: Makefile,v 1.29 1999/09/10 00:05:32 edwardc Exp $

# The home directory for user 'bbs'
BBSHOME=$(BBS_HOME)

# The uid/gid info for user 'bbs' and 'bbsadm'
BBSUID=$(BBS_UID)
BBSGRP=$(BBSGID)

OS_DEF   = $(OS_DEF)
CC       = $(CC)
CFLAGS   = $(CFLAGS)
LIBS     = $(LIBS)

INSTALL  = $(INSTALL)

CSIE_DEF = $(CSIE_DEF)

#############################################################
###        You needn't modify the following stuff      ######
#############################################################
PROGNAME = bbsd
CFILE	 = admintool.c announce.c bbs.c bbsd.c bbsgopher.c bcache.c bm.c \
		   boards.c chat.c comm_lists.c delete.c edit.c fileshm.c goodbye.c \
		   help.c io.c list.c mail.c main.c maintain.c modetype.c more.c \
		   namecomplete.c pass.c paging.c postheader.c read.c record.c \
		   register.c screen.c sendmsg.c stuff.c talk.c term.c \
		   userinfo.c vote.c xyz.c

COBJS	 = admintool.o announce.o bbs.o bbsd.o bbsgopher.o bcache.o bm.o \
		   boards.o chat.o comm_lists.o delete.o edit.o fileshm.o goodbye.o \
		   help.o io.o list.o mail.o main.o maintain.o modetype.o more.o \
		   namecomplete.o pass.o paging.o postheader.o read.o record.o \
		   register.o screen.o sendmsg.o stuff.o talk.o term.o \
		   userinfo.o vote.o xyz.o

#SO		 = paging.so	#  µ—È÷– ..

GARBAGE  = a.out core installchatd bbs.chatd chatd bbsrf thread *~ *.BAK

# The -DINVISIBLE makes bbs users invisible to 'finger' and 'who'.
# Simply remove it if you don't want this feature.

DEFINES	=  $(CSIE_DEF) $(OS_DEF)
EXTRADEF = -DHAVE_VERSION_H

# to active no admin tool bbs client, add -DWITHOUT_ADMIN_TOOLS
# into EXTRADEF, program will generate an admintool less bbsd.
 
.SUFFIXES: .o .c .so

.c.o:	;	$(CC) $(CFLAGS) $(DEFINES) $(EXTRADEF) -c $*.c
.o.so:	;	ld -s -G $*.o -o $*.so -L../lib -lBBS

#--------------------- Dependency starts here -------------------
all: version.h $(PROGNAME) bbs.chatd bbsrf thread expire
	rm -f ../include/version.h

so: $(SO)

version.h:
	echo "Generate version info.."
	sh ver.sh ../include/version.h

$(PROGNAME): $(COBJS)
	$(CC) -o $(PROGNAME) $(CFLAGS) $(CSIE_DEF) $(COBJS) $(LIBS) -L../lib -lBBS
	@echo "Program size: `cat $(PROGNAME)|wc -c` bytes"

#disable bbsrf, it's can be replaced by in.zbbsd and native bbsd
#bbsrf: bbsrf.c 
#	$(CC) $(CFLAGS) -o bbsrf $(DEFINES) bbsrf.c $(LIBS)
bbsrf:
	@echo " "
	@echo "** bbsrf is useless, it can be replaced by in.zbbsd and native bbsd **"
	@echo " "

bbs.chatd: station.c
	$(CC) $(CFLAGS) -o chatd $(OS_DEF) station.c $(LIBS) -L../lib -lBBS

thread: thread.c record.c
	$(CC) $(CFLAGS) -o thread $(OS_DEF) record.c thread.c $(LIBS)

install:
	sh Install.sh

installbbs: bbsd
	$(INSTALL) -s -m 550 -g $(BBSGRP) -o $(BBSUID) bbsd $(BBSHOME)/bin/bbsd.new
	-rm -f $(BBSHOME)/bin/bbsd.old
	-mv $(BBSHOME)/bin/bbsd $(BBSHOME)/bin/bbsd.old
	mv $(BBSHOME)/bin/bbsd.new $(BBSHOME)/bin/bbsd

installthread: thread
	$(INSTALL) -s -m 550 -g $(BBSGRP) -o $(BBSUID) thread $(BBSHOME)/bin/thread.new
	-rm -f $(BBSHOME)/bin/thread.old
	-mv $(BBSHOME)/bin/thread $(BBSHOME)/bin/thread.old
	mv $(BBSHOME)/bin/thread.new $(BBSHOME)/bin/thread

installchatd: bbs.chatd
	$(INSTALL) -s -m 550 -g $(BBSGRP) -o $(BBSUID) chatd $(BBSHOME)/bin/chatd.new
	-rm -f $(BBSHOME)/bin/chatd.old
	-mv $(BBSHOME)/bin/chatd $(BBSHOME)/bin/chatd.old
	mv $(BBSHOME)/bin/chatd.new $(BBSHOME)/bin/chatd

# Disable bbsrf .. 
#installbbsrf: bbsrf
#	$(INSTALL) -s -m 550 -g $(BBSGRP) -o $(BBSUID) bbsrf $(BBSHOME)/bin/bbsrf
#
#installrootbbsrf: bbsrf
#	$(INSTALL) -s -m 4555 -g bin -o root bbsrf $(BBSHOME)/bin/bbsrf
#

clean: /tmp
	-rm -fr $(GARBAGE) $(SO) $(COBJS) $(PROGNAME) $(LNFILES) \
	../include/version.h expire

cleanall: clean
	-rm -f Install.sh
	-rm -f ../include/config.h
	-rm -f ../include/chat.h
	-rm -f bbs bbsrf chatd thread

tags: /tmp
	ctags $(CFILE)

update: installbbs installchatd installthread installexpire

expire: expire.c record.c
	$(CC) $(CFLAGS) $(OS_DEF) -o expire expire.c

modestat:
	$(CC) $(CFLAGS) -o modestat modestat.c

installexpire: expire
	$(INSTALL) -s -m 550 -g $(BBSGRP) -o $(BBSUID) expire $(BBSHOME)/bin/expire.new
	-rm -f $(BBSHOME)/bin/expire.old
	-mv $(BBSHOME)/bin/expire $(BBSHOME)/bin/expire.old
	mv $(BBSHOME)/bin/expire.new $(BBSHOME)/bin/expire

# DO NOT DELETE
