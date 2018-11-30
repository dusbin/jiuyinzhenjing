#! /bin/bash
echo "begin..."
set -x
path=$1
let bottle=$2
let min=$3
let max=$4
function fen(){
	ii=1
	while [ $ii -le $bottle ]
	do
		touch $path.$ii
		let ii++
	done
	sleep 2
	let step=(max-min)/bottle
	echo $step
	sleep 2
	i=0
	while read line
	do
		let get=${line}
		let i=(get-min)/step
		echo $i
		echo "${line}">>$path.$i
		sleep 1
	done < $path
}
function paixu(){
	i=1
	while [ $i -le $bottle+1 ]
	do
		cat $path.$i|sort -n >$path.$i.sort
		let i++
	done
}
function he(){
	i=1
	while [ $i -le $bottle+1 ]
	do
		cat $path.$i.sort >>$path.end
		let i++
	done
}
function main(){
	fen
	paixu
	he
}
main
set +x
echo "end..."

