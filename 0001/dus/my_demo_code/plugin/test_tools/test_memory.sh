#! /bin/bash
ps aux |grep -E 'USER|main'|grep -v grep
while :
do
	echo "-------------------------------------------------------------"
	echo "-------------------------------------------------------------" >>/opt/plugin/memory
	ps aux |grep -E 'USER|main'|grep -v grep
	ps aux |grep -E 'USER|main'|grep -v grep >>/opt/plugin/memory
	sleep 5;
done

