#!/bin/bash
echo "begin"
root_dir=`pwd`
make clean
make
mkdir /dir
cp test /dir/
cd /dir
ln test dus
ln test uxsino
ln test dxx
#ln -s test dus
#ln -s test uxsino
#ln -s test dxx
echo "begin test"
echo "exec ./dus"
./dus
echo "exec ./uxsino"
./uxsino
echo "exec ./dxx"
./dxx
cd $root_dir
rm -rf /dir/
make clean
