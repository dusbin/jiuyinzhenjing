#! /bin/bash
#	$1	开始版本
#	$2	结束版本
#	$3	svn地址
pwd_path=`pwd`
if [ $1"x" = "x" ];then
	i=1
	ii=1
else 
	i=$1
	ii=$1
fi
if [ $2"x" = "x" ];then
	end=89
else 
	end=$2
fi
if [ $3"x" = "x" ];then
	url=http://code.taobao.org/svn/tinixdev/
else 
	url=$3
fi

while [ $ii -le $end ]
do
	#if then
		mkdir $ii
		let ii+=1;
	#fi
done
#mkdir {$1..$2}
while :
do
	if [ $i -le $end ];then
	cd $pwd_path/$i
		svn export -r $i $url
		echo "success svn r"$i
		let i+=1;
		sleep 1s
	else 
		exit 0;
	fi
done
