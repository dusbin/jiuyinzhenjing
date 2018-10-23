#! /bin/bash
#echo "begin..."
i=0
ii=0
while [ $i -le 100 ]
do
    #let ii=i%3
    let ii=i%2
    case $ii in
	0)
	echo " 0.0"
	echo "  -"
	;;
    	1)
	echo " -.-"
	echo "  -"
	;;
	2)
	echo " ^.^"
	echo "  -"
	;;
	*)
	;;
    esac
    let i++
    sleep 1
    clear
done
printf "\n"
echo "end..."










