# ! /bin/bash
echo "begin"
rm -rf dus_a9rootfs.ext3
dd if=/dev/zero of=dus_a9rootfs.ext3 bs=1M count=128
mkfs.ext3 dus_a9rootfs.ext3
#mount -t ext3 dus_a9rootfs.ext3 tmpfs/ -o loop
mount -t ext3 dus_a9rootfs.ext3  tmpfs/ -o rw
cp -r rootfs/* tmpfs/
umount tmpfs
#umount `df -h |grep loop|awk -F' ' '{printf$1}'`
sleep 3 
chmod 777 dus_a9rootfs.ext3
qemu-system-arm -M vexpress-a9 -m 512M -kernel linux-3.12.10/arch/arm/boot/zImage -nographic -append "root=/dev/mmcblk0 console=ttyAMA0" -sd dus_a9rootfs.ext3
