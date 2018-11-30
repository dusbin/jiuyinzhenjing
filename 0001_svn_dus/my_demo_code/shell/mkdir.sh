#! /bin/bash
echo "begin..."
rm -rf testdir/
i=1
while :
do
mkdir -p testdir
echo $?
cd testdir
echo $?
touch "testfile"$i
echo $?
echo "deepin"$i
let i+=1;
done











