#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<string.h>
#define MAX_PATH 1024
void fsize(char *name);
void dirwalk(char *dir){
	char name[MAX_PATH];
	struct dirent *dp;
	DIR *dfd;
	if((dfd = opendir(dir)) == NULL){
		fprintf(stderr,"dirwalk:can't open %s\n",dir);
		return;
	}
	while((dp = readdir(dfd)) != NULL){
		if(strcmp(dp->d_name,".")==0||strcmp(dp->d_name,"..")==0)
			continue;
		if(strlen(dir)+strlen(dp->d_name)+2 > sizeof(name))
			fprintf(stderr,"dirwalk:name %s %s too long\n",dir,dp->d_name);
		else{
			sprintf(name,"%s/%s",dir,dp->d_name);//拼接下一级目录
			fsize(name);//进入到下一级目录
		}
	}
	closedir(dfd);
}
void fsize(char *name){
	struct stat stbuf;
	if(stat(name,&stbuf) == -1){
		fprintf(stderr,"fsize:can't access %s\n",name);
		return;
	}
	if((stbuf.st_mode & S_IFMT) == S_IFDIR)
		dirwalk(name); //如果是目录，进入目录。实现函数递归
	printf("%8ld %s\n",stbuf.st_size,name);//主要打印
}
int main(int argc,char **argv){
	printf("begin\n");
	if(argc == 1)
		fsize(".");//如果没有输入目录参数，定义为当前目录
	else
		while(--argc > 0)
			fsize(*++argv);//函数主入口，进入要打印的目录，可以是多个目录作为参数
	return 0;
	printf("end\n");
	return 0;
}
//这个程序不如ls -R健壮，可能存在死循环，什么情况下存在死循环，当创建文件目录链接时

