#! /bin/bash
echo "begin"
make clean >/dev/null
make >/dev/null
echo "----------- no flag --------------"
./hello
make clean >/dev/null
make CFLAGS=-DDUS >/dev/null
echo "--------- flag is dus ------------"
./hello
make clean >/dev/null
make CFLAGS=-DUXSINO >/dev/null
echo "------- flag is uxsino -----------"
./hello
make clean >/dev/null
make CFLAGS="-DDUS -DUXSINO" >/dev/null
#make CFLAGS='-DDUS -DUXSINO' >/dev/null #is also ok
echo "----- flag is dus&uxsino ---------"
./hello
make clean >/dev/null
echo "end"