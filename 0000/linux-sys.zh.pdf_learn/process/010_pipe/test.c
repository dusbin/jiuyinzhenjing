#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define MAXLINE 80
int main(){
	printf("begin\n");
	int n;
	int fd[2];
	pid_t pid;
	char line[MAXLINE];
	if(pipe(fd) < 0){
		perror("pipe");
		exit(1);
	}
	if((pid = fork()) < 0){
		perror("fork");
		exit(1);
	}
	if(pid > 0){/*parent 写数据到管道*/
		close(fd[0]);
		write(fd[1],"hello world",11);
		wait(NULL);
	}else{ /*child 从管道中读数据*/
		close(fd[1]);
		n = read(fd[0],line,MAXLINE);
		write(STDOUT_FILENO,line,n);
		line[n] = '\0';
		printf("\nline = [%s]\n",line);
	}
	printf("end\n");
	return 0;
}

