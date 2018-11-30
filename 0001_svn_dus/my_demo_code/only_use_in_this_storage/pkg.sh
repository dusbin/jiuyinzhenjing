#! /bin/bash
if [[ -z $1 ]];then
	echo "Need a parameter, the parameter is the package name...."
	exit 0
fi
ROOT_PATH=`pwd`
SerialNumber=""
mount_port=`ls /proc/scsi/usb-storage/ |awk -F' ' '{printf$1}'`
SerialNumber=`cat /proc/scsi/usb-storage/${mount_port}|grep Serial|awk -F' ' '{printf$3}'`
passwd=$SerialNumber"xiansf"
echo $passwd
tar -zcvf - $1 |openssl des3 -salt -k $passwd|dd of=$1.tar.gz


