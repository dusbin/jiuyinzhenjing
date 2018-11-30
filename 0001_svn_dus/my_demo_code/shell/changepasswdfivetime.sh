#! /bin/bash
set -x
execuser=$1
changeuser=$2
pass=$3
function inputcheck(){
	if [ $execuser"x" = "x" ];then
		echo "use:./changepasswdfivetime.sh execuser changeuser passwd"
		exit -1
	fi
	if [ $changeuser"x" = "x" ];then
		echo "use:./changepasswdfivetime.sh execuser changeuser passwd"
		exit -1
	fi
	if [ $pass"x" = "x" ];then
		echo "use:./changepasswdfivetime.sh execuser changeuser passwd"
		exit -1
	fi
	ret=`cat /etc/passwd|awk -F':' '{print$1}'|grep -w "$execuser"`
	if [$execuser"x" = $ret"x"];then
		echo $execuser
	else
		echo $execuser"is not exist"
		useradd -G root $execuser
	fi
	ret=`cat /etc/passwd|awk -F':' '{print$1}'|grep -w "$changeuser"`
	if [$changeuser"x" = $ret"x"];then
		echo $execuser
	else
		echo $changeuser"is not exist"
		useradd -G root $changeuser
	fi
}
function changepassword(){
	#passwdold=$1
	#passwdnew=$2
	passwdnew=$1
	su - $execuser -c "echo $passwdnew|passwd --stdin $changeuser" #<<EOF
	#$passwdold \
	#$passwdnew \
	#$passwdnew \
	#EOF"
}
function main(){
	echo "begin..."
	inputcheck
	#changepassword $pass "Qqlist170*220" 
	changepassword "Qqlist170*220" 
	changepassword "QQlist170*220"
	changepassword "QQList170*220"
	changepassword "QQlIst170*220"
	changepassword "QQliSt170*220"
	changepassword "QQlisT170*220"
	changepassword "QQliTs170*220"
	changepassword "QQlTis170*220"
	changepassword "QTQlis170*220"
	echo "end..."
}
main
set +x
