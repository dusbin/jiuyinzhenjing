#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "gettimestring.h"
#include "comm.h"
int main(){
	char strtime[STRINGALLTIMELEN] = {0};
	int ret = FUNC_UNDEFINE;
	ret = gettimestring(strtime);
	printf("ret = %d,strtime = [%s]\n",ret,strtime);
	return FUNC_OK;
}

