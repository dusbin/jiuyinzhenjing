#include "comm_define.h"
#include <stdio.h>
#include <string.h>
int func(int a, int b){
    if(b == 0)
	return -9999999;
    return (a / b);
}
int getmod(char *output){
        strcpy(output,"a");
	return FUNC_OK;
}

int exec(char *input,int input_len,char *output,int output_len){
	printf("main says:[%s]\n",input);
	strcpy(output,"i am plugin[libhostinfo]");
	return FUNC_OK;
}

