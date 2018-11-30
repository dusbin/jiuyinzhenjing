#! /bin/bash 
gcc -fPIC -shared -o libjiafa.so jiafa.c
gcc -fPIC -shared -o libjianfa.so jianfa.c
gcc -fPIC -shared -o libchengfa.so chengfa.c
gcc -fPIC -shared -o libchufa.so chufa.c
gcc test.c -L. -ljiafa -ljianfa -lchengfa -lchufa