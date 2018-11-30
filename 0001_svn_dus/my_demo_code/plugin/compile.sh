#! /bin/bash
#gcc -fPIC -shared source.c -o libsource.so
ROOT=`pwd`
INSTALL_DIR=/opt/plugin
LIBS_DIR=libs
MODULE=module
MAIN=main
INC=inc
SRC=src
MAKE=make
LIBADD=libadd
LIBDIV=libdiv
LIBMUL=libmul
LIBSUB=libsub
LIBHOSTINFO=libhostinfo
BIN=bin

rm -rf $INSTALL_DIR/
mkdir -p $INSTALL_DIR/$LIBS_DIR/

mkdir -p $ROOT/$BIN
cd $ROOT/$MODULE/$LIBSUB/$SRC
gcc -fPIC -shared sub.c -o libsub.so
#mv libsub.so $ROOT/$BIN
mv libsub.so $INSTALL_DIR/$LIBS_DIR
cd $ROOT/$MODULE/$LIBMUL/$SRC
gcc -fPIC -shared mul.c -o libmul.so
#mv libmul.so $ROOT/$BIN
mv libmul.so $INSTALL_DIR/$LIBS_DIR
cd $ROOT/$MODULE/$LIBADD/$SRC
gcc -fPIC -shared add.c -o libadd.so
#mv libadd.so $ROOT/$BIN
mv libadd.so $INSTALL_DIR/$LIBS_DIR
cd $ROOT/$MODULE/$LIBDIV/$SRC
gcc -fPIC -shared div.c -o libdiv.so
#mv libdiv.so $ROOT/$BIN
mv libdiv.so $INSTALL_DIR/$LIBS_DIR
cd $ROOT/$MODULE/$LIBHOSTINFO/$SRC
gcc -fPIC -shared libhostinfo.c -o libhostinfo.so -I ../../../main/comm/inc/
mv libhostinfo.so $INSTALL_DIR/$LIBS_DIR
cd $ROOT/$MAIN/$MAKE/
make clean
make
#mv main $ROOT/$BIN
cp main $INSTALL_DIR
make clean
#ls -l $ROOT/$BIN
rm -rf $ROOT/$BIN
ls -l $INSTALL_DIR
ls -l $INSTALL_DIR/$LIBS_DIR/
cd $INSTALL_DIR
