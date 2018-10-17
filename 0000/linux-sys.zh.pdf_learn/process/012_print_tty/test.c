#include<stdio.h>
#include<stdlib.h>
int main(){
	printf("begin\n");
	printf("fd 0:%s\n",ttyname(0));
	printf("fd 1:%s\n",ttyname(1));
	printf("fd 2:%s\n",ttyname(2));
	pid_t pid;
	if((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}else if (pid == 0) {
		printf("child process PID is %d\n",getpid());
		printf("Group ID is %d\n",getpgrp());
		printf("Group ID is %d\n",getpgid(0));
		printf("Group ID is %d\n",getpgid(getpid()));
		exit(0);
	}
	sleep(3);
	printf("parent process PID is %d\n",getpid());
	printf("Group ID is %d\n",getpgrp());
	printf("end\n");
	return 0;
}

