#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main(){
	printf("begin\n");
	//调用exec不产生新进程，调用前后id未改变
	execlp("ps","ps","-o","pid,ppid,pgrp,session,tpgid,comm",NULL);
	perror("exec ps");
	printf("end\n");
	return 0;
}

