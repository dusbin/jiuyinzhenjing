#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "comm.h"
int exec_command(char *path,char *command){
	int ret = FUNC_UNDEFINE;
	if(path == NULL || command == NULL){
		return FUNC_ERR_INPUT_NULL;
	}
	if(access(path, F_OK))	return	FUNC_WARNNING_INPUT_PATH_NO_EXIST;
	char cmd[1024] = {0};
	sprintf(cmd,"cd \"%s\";%s",path,command);
	ret = system(cmd);
	if(ret == 0){
		return FUNC_OK;
	}else{
		return FUNC_ERR_EXEC;
	}
}

