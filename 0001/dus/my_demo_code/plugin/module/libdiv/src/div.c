#include "../../../main/comm/inc/Comm_define.h"
#include <string.h>
int func(int a, int b){
    if(b == 0)
	return -9999999;
    return (a / b);
}
int getmod(char *output){
        strcpy(output,"/");
	return FUNC_OK;
}


