#include"parse.h"
#include<stdio.h>
#include<string.h>
/*
 * 读取命令
 * 成功返回0，失败/读取结束符EOF返回-1
 */
int read_cmd(){
	char cmd[1024+1];
	memset(cmd,0x00,sizeof(cmd));
	if(fgets(cmd,1024,stdin)==NULL){
		return -1;
	}
	return 0;
}
/*
 * 解析命令
 * 成功返回解析的命令个数，失败返回-1
 */
int parse_cmd(){
	return 0;
}
/*
 * 执行命令
 * 成功返回0，失败返回-1
 */
int exec_cmd(){
	return 0;
}

/*
 * shell主循环
 */
void shell_loop(){
	while(1){
		printf("[minishell$] ");
		fflush(stdout);
		if(read_cmd()!= 0){
			break;
		}
		parse_cmd();
		exec_cmd();
	}
	printf("\nexit\n");
	return;
}