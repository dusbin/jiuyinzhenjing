#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
int main(int argc,char *argv[]){
	printf("begin\n");
	//调用exec不产生新进程，调用前后id未改变
	int fd;
	if(argc != 2){
		fputs("usage:test file\n",stderr);
		exit(1);
	}
	fd = open(argv[1],O_RDONLY);
	if(fd<0){
		perror("open");
		exit(1);
	}
	dup2(fd,STDIN_FILENO);
	close(fd);
	//将命令行参数当作文件名打开，将标准输入重定向到这个文件，然后调用
	//exec执行upper程序，这时原来打开的文件描述符仍然是打开的，upper程序只负责从标准输
	//入读入字符转成大写，并不关心标准输入对应的是文件还是终端。
	execl("./upper","upper",NULL);
	perror("exec ./upper");
	exit(1);
	execlp("ps","ps","-o","pid,ppid,pgrp,session,tpgid,comm",NULL);
	perror("exec ps");
	printf("end\n");
	return 0;
}

