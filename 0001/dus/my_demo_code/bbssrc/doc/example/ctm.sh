#!/bin/sh
# 此 shell script 利用 ctm 一次更新多个 ctm Delta
# NAME 为数字之前的档名
# 用法 : 将 ctm.sh 放到要解开的目录 ex. /home/bbsadm/bbssrc，CTM档假设放在
#	 /home/bbsadm/CTM，想要更新编号 2 - 23 的 Delta
#
#	cd /home/bbsadm/bbssrc
#	ctm.sh /home/bbsadm/CTM 2 23
#					任何问题请 mail: skyo@mgt.ncu.edu.tw
#
#   $Id: ctm.sh,v 1.2 1999/06/06 04:35:24 skyo Exp $    
NAME="fb3src-"
A=$2
while [ $3 -ge $A ]
do
	ctm -v $1/$NAME$A.gz
        A=`expr $A + 1 `
done
