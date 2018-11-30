#include "../../../main/comm/inc/Comm_define.h"
#include <string.h>
int func(int a,int b){
    return (a + b);
}
int getmod(char *output){
        strcpy(output,"+");
	return FUNC_OK;
}

