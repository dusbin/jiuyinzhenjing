# Generated automatically from Makefile.in by configure.
# Makefile of mail2bbs for Firebird BBS 3.0
#
# $Id: Makefile.in,v 1.2 1999/06/05 08:11:02 edwardc Exp $

CC = gcc
BBS_SRC = ../../src
RECORD_OBJ = $(BBS_SRC)/record.c
CFLAGS = -I../../include
INSTALLDIR = /home/bbs/bin
PERL = /usr/bin/perl
DOMAIN = test.dus

all: mail2bbs fillperl install

mail2bbs: mail2bbs.c $(RECORD_OBJ)
	$(CC) $(CFLAGS) -DDOMAIN_NAME=\"'$(DOMAIN)'\" -DBBSNAME=\"'dus'\" \
	-Xlinker -Bstatic -o mail2bbs mail2bbs.c qp_conv.c mail-list.c $(RECORD_OBJ)

install: mail2bbs
	install -g bbs -m 4750 -o bbs mail2bbs $(INSTALLDIR)
	install -g bbs -m  750 -o bbs bbsmail     $(INSTALLDIR)
	install -g bbs -m  750 -o bbs mailgate.pl $(INSTALLDIR)

fillperl:
	rm -fr bbsmail mailgate.pl
	sed -e "s:@PERL@:${PERL}:" bbsmail.in | \
	sed -e "s:@BBSUID@:9999:" | \
	sed -e "s:@BBSGID@:99:" | \
	sed -e "s:@BBSHOME@:/home/bbs:" > bbsmail
	sed -e "s:@PERL@:${PERL}:" mailgate.pl.in | \
	sed -e "s:@BBSDOMAIN@:$(DOMAIN):" > mailgate.pl

clean:
	rm -fr *.o *~ mail2bbs
