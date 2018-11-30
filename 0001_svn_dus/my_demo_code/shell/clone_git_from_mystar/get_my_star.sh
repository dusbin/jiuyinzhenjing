#! /bin/bash
#	1.本程序在ubuntu14.04.1下测试
#	2.需要使用到的工具有 wget git 使用前请自行安装
#	3.github网站接口不变的情况下可以继续使用
#		接口描述如下
#		https://github.com/Bin-DuS?page=1&tab=stars
#		https://github.com/username?page=N&tab=stars
#set -x
BD="Bin-DuS"
if [ $1"x" = "x" ];then
	name=$BD
else 
	name=$1
fi
echo $name
#获取mystargit
function get_list(){
	#生成要wget的网址链接
	rm -rf $name*
	rm -rf star_list
	i=1
	while [ $i -le 20 ]
	do
		echo "https://github.com/"$name"?page="$i"&tab=stars" >>star_list
		let i++
	done
	#获取网页数据
	cat star_list | while read line
	do 
		echo "File:${line}"
		wget ${line}
	done
	rm -rf star_list
	#将wget到的数据过滤获取作者仓库名
	cat $name* |grep %2F|grep -v $name|awk -F'=' '{print$3}' >mystar
	rm -rf $name*
	#转换%2F为/ 过滤杂质
	sed -i 's#%2F#/#' mystar #将%2F转换为/
	sed -i 's#%2F#/#' mystar #将%2F转换为/
	cat mystar |awk -F'"' '{print$1}' >mystar.bak
	mv mystar.bak mystar
	#生成完整的star仓库下载路径
	while read line
	do
		echo "https://github.com${line}.git" >>mystargit
	done < mystar
	rm -rf mystar
	cp mystargit mystargit.bak
}
function git_clone(){
	#git clone下载代码
	while [ 1 = 1 ]
		do
		
		if [  -f "gitting_repo" ] ; then #/opt/clone/git/gitting_repo文件存在
			rm -rf `cat gitting_repo|awk -F'/' '{print$(NF-1)"_"$NF}'|awk -F '.' '{printf$1}'`/`cat gitting_repo|awk -F'/' '{print$NF}'|awk -F '.' '{printf$1}'` #删除clone不全的文件夹
		fi
		repo=`head -1 mystargit |awk -F'#' '{printf$1}'`
		if [ $repo"x" = "x" ];then
			return
		fi
		path=`echo "$repo"|awk -F'.' '{printf$(NF-1)}'|awk -F'/' '{printf$(NF-1)"_"$NF}'`
		mkdir $path  2>/dev/null    #针对不同用户相同仓库名进行区分，创建以用户名为目录的文件夹
		cd $path         #
		echo $repo >gitting_repo
		git clone $repo
		rm gitting_repo
		cd ..                                                   #
		cat mystargit | grep -v $repo >mystargit.bak1
		mv mystargit.bak1 mystargit
	done
	echo "end..."
}
function main(){
	echo "begin..."
	if [ ! -f "mystargit" ] ; then #/opt/clone/git/mystargit文件不存在
		get_list
	fi
	git_clone
}
main
#set +x
#rm -rf /opt/clone/git/mystargit



#参考信息
#wget https://github.com/Bin-DuS?page=1&tab=stars
#https://github.com/Bin-DuS?tab=stars
#cat Bin-DuS\?tab\=stars |grep %2F|grep -v Bin-DuS|awk -F'=' '{print$3}'
