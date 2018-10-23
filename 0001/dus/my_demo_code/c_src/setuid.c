/*
 *检查凝思磐石系统是否以无root模式启动，设置用户其他用户的权限
 *https://blog.csdn.net/qc_liu/article/details/40984779
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include<sys/types.h>
#include<unistd.h>
int main(){//以root执行
	int uid = getuid();
	char buff[20] = {0};
	//system("echo 123 > /opt/test");
	FILE *fp = NULL;
	printf("uid = %d\n",uid);
	printf("not root\n");
	fp=popen("cat /opt/test","r"); //文件/opt/test为dzb1用户600权限，其他用户无读写权限
	if(fp ==NULL){
		return -1;
	}
	memset(buff,0x00,sizeof(buff));
	fgets(buff,sizeof(buff),fp);
	pclose(fp);
	fp = NULL;
	printf("buff = %s\n",buff);
	if(!setuid(1001)){//降权为1001用户
		printf("setuid successfully!\n");
	}else{
		printf("setuid error!\n");
	}
	printf("root\n");
	fp=popen("cat /opt/test","r");//无权限访问
	if(fp ==NULL){
		return -1;
	}
	memset(buff,0x00,sizeof(buff));
	fgets(buff,sizeof(buff),fp);
	pclose(fp);
	fp = NULL;
	printf("buff = %s\n",buff);
	if(!setuid(uid)){ //升权为root权限，失败，低权限用户不能升权限为高权限用户
		printf("setuid successfully!\n");
	}else{
		printf("setuid error!\n");
	}
	printf("not root\n");
	fp=popen("cat /opt/test","r");
	if(fp ==NULL){
		return -1;
	}
	memset(buff,0x00,sizeof(buff));
	fgets(buff,sizeof(buff),fp);
	pclose(fp);
	fp = NULL;
	printf("buff = %s\n",buff);
	return 0;
}

