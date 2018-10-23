#! /bin/bash
#	使用时确保watchdog没有启动，防止watchdog拉起server
#	当server中有僵尸进程时kill掉server，及时排查发生僵尸进程的原因
#
#
for((i=1; i > 0; i=1));do
	if [  -f "/opt/infozombie" ] ; then 
	rm -rf /opt/infozombie
	fi
	ps -ef | grep `ps -ef | grep ListenHostServer | grep -v grep |awk -F' ' '{printf$2}'`| grep defunct > /opt/infozombie
	buff=`cat /opt/infozombie`
	clear
	echo "runing...."
	#echo "end buff"
	if [ "$buff"x != ""x ];then
		kill -9 `ps -ef|grep ListenHostServer|grep -v grep |awk '{print $2}'`
		echo "I killed the ListenHostServer hahaha,you have a zombie or same zombie,please check and deal with the zombie!!!"
		exit 0
	fi
	sleep 1
done 
