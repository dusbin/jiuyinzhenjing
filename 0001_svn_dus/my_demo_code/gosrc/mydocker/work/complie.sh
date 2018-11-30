#! /bin/bash
# export GOPATH=/go
# export GOROOT=/go  ~/.bashrc文件 默认的GOROOT不用改
echo "begin..."
rm -rf /root/busybox.tar
rm -rf /usr/lib/go-1.6/src/github.com/
mkdir -p /usr/lib/go-1.6/src/github.com/xianlubird/
cp -r mydocker-dev/ /usr/lib/go-1.6/src/github.com/xianlubird/
mv /usr/lib/go-1.6/src/github.com/xianlubird/mydocker-dev /usr/lib/go-1.6/src/github.com/xianlubird/mydocker
cd /usr/lib/go-1.6/src/github.com/xianlubird/mydocker/
go build .
rm -rf /root/busybox
cd /root/
cp -r /mnt/hgfs/dus/branches/my_demo_code/gosrc/mydocker/work/busybox.tar .
#cp -r busybox.tar /root/
#mkdir -p /root/busybox
echo "end..."
