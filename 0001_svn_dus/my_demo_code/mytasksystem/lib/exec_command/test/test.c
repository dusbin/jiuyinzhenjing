#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "exec_command.h"
#include "comm.h"
int main(){
	int ret = FUNC_UNDEFINE;
	ret = exec_command("/opt/","mkdir dzb");
	printf("ret = %d\n",ret);
	ret = exec_command("/opt/dzb","touch {1..10000}");
	printf("ret = %d\n",ret);
	ret = exec_command("/opt/","tar -cvf dzb.tar.gz dzb/ >/dev/null");
	printf("ret = %d\n",ret);
	ret = exec_command("/opt/dzb1","tar -cvf dzb.tar.gz dzb/ >/dev/null");
	printf("ret = %d\n",ret);
	return FUNC_OK;
}

