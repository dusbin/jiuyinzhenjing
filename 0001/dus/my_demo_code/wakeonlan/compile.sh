#! /bin/bash
#gcc -fPIC -shared source.c -o libsource.so
convert -fill green -pointsize 40 -draw 'text 20,50 "www.baidu.com"' $1 $1.bak
display $1.bak
rm $1.bak

