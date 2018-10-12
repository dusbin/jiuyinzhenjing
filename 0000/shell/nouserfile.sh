#! /bin/bash
set -x
function main(){
	cd $1
	useradd testandtest
	touch test
	chown testandtest:testandtest test
	userdel testandtest
	ls -l test
}
main $1
