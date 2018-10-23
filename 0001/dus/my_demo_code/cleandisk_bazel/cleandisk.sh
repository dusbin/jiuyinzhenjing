#! /bin/bash
echo "begin..."
rm -rf /root/Desktop/cleandisk/
cp -r cleandisk/ /root/Desktop/
cd /root/Desktop/cleandisk/
bazel build //main:cleandisk
cd -
echo "end..."










