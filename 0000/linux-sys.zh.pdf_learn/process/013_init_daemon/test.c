#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
int init_daemon(void){
	pid_t pid;
	umask(0);//4 /* clear file mode creation mask */
	/* parent exits , child continues */
	if((pid = fork()) < 0){//1
		printf("error fork()");
		return -1;
	}
	else if(pid != 0){
		printf("error != 0");//正确的分支
		exit(0);
	}
	setsid();//2 /* become session leader */
	/*
	if(chdir("/")<0){ //3
		exit(1);
	}
	*/
	close(1);//5
	//close(0);
	
	return 0;
}
int main(){
	printf("begin\n");
	if(init_daemon() == -1){
		exit(0);
	}
	while(1){
		sleep(1);
	}
	printf("end\n");
	return 0;
}
/*
1. 创建子进程，父进程退出
所有工作在子进程中进行
  形式上脱离了控制终端
2. 在子进程中创建新会话
  setsid()函数
  使子进程完全独立出来，脱离控制
3. 改变当前目录为根目录
  chdir()函数
  防止占用可卸载的文件系统
  也可以换成其它路径
4. 重设文件权限掩码
  umask()函数
  防止继承的文件创建屏蔽字拒绝某些权限
  增加守护进程灵活性
5. 关闭文件描述符
  继承的打开文件不会用到，浪费系统资源，无法卸载
6. 开始执行守护进程核心工作
7. 守护进程退出处理
 */

