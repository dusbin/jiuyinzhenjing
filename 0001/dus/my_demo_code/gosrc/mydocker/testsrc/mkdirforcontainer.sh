#! /bin/bash
echo "begin..."
rm -rf /rootfs/
mkdir -p /rootfs
mkdir -p /rootfs/bin
mkdir -p /rootfs/dev
mkdir -p /rootfs/etc
mkdir -p /rootfs/home
mkdir -p /rootfs/lib
mkdir -p /rootfs/lib64
mkdir -p /rootfs/mnt
mkdir -p /rootfs/opt
mkdir -p /rootfs/proc
mkdir -p /rootfs/root
mkdir -p /rootfs/run
mkdir -p /rootfs/sbin
mkdir -p /rootfs/sys
mkdir -p /rootfs/tmp
mkdir -p /rootfs/usr
mkdir -p /rootfs/var
cp -r /bin/* /rootfs/bin/
cp -r /sbin/* /rootfs/sbin/
cp -r /lib/* /rootfs/lib/
cp -r /lib64/* /rootfs/lib64/
cp -r /etc/* /rootfs/etc/
cp /root/.* /rootfs/root/











