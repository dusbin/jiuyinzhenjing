#! /bin/bash
#gcc -fPIC -shared source.c -o libsource.so
gcc -fPIC -shared sub.c -o libsub.so
gcc -fPIC -shared mul.c -o libmul.so
gcc -fPIC -shared add.c -o libadd.so
gcc -fPIC -shared div.c -o libdiv.so
gcc -rdynamic -o test test.c -ldl
