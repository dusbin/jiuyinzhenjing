#! /bin/bash
function rand(){
	min=$1
	max=$(($2-$min+1))
	num=$(($RANDOM+1000000000)) #增加一个10位的数再求余
	echo $(($num%$max+$min))
}
function main(){
	echo "begin..."
	i=0
	
	while [ $i -le 1000000000 ]
	do
		rnd=$(rand  400000 500000)
		echo $rnd >>/opt/data
		let i++
	done
echo "end..."
}
main









