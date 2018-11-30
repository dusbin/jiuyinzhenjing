#include <stdio.h>
#include <sys/time.h>
#include "comm.h"
int gettimestring(char *time){
	if(time == NULL){
		return FUNC_ERR_INPUT_NULL;
	}
	/*
	if(sizeof(time) < STRINGALLTIMELEN){
		return FUNC_ERR_INPUT_BUFF_NO_ENOUGH;
	}
	*/
    struct timeval tv;
    gettimeofday(&tv,NULL);
	sprintf(time,"%ld",tv.tv_sec*1000000 + tv.tv_usec);
    return FUNC_OK;
}

