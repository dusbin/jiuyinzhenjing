#! /bin/bash
#shell脚本执行三次删除脚本，加密后不可用。
#实现原理，获取脚本最后一次访问时间，并写入到脚本，当访问三次删除脚本
root_dir=`pwd`
shell_name='test_rm.sh'
shell_name_bak='test_rm_bak.sh'
cp $root_dir/$shell_name $root_dir/$shell_name_bak
time=`cat $root_dir/$shell_name|grep 000000000|grep -v grep |wc -l`
echo $time
if [ $time == 3 ];then
    rm $root_dir/$shell_name_bak
    rm $0
	exit 0;
fi
cat $root_dir/$shell_name_bak
echo $root_dir
info=`stat $root_dir/$shell_name|grep Access|grep 0000|grep -v grep |awk -F' ' '{printf$2$3}'`
echo "#${info}" >>$root_dir/$shell_name_bak
echo "test"
rm $0
mv $root_dir/$shell_name_bak $root_dir/$shell_name


