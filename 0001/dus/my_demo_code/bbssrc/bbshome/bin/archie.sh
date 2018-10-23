#!/bin/sh
# $Id: archie.sh,v 1.3 1999/07/25 06:19:10 biboman Exp $
echo
echo "ËÑÑ°ÖÐ, ÇëÉÔºò....."
echo "Firebird BBS ARCHIE Service." `date` > /home/bbs/tmp/archie.$2.$3
echo "Search for: $1" >> /home/bbs/tmp/archie.$2.$3
exec /usr/local/bin/archie $1 >> /home/bbs/tmp/archie.$2.$3
