#include "define.h"
int chufa(int a,int b,int* c){
	if(b != 0){
		c = a/b;
		return FUNC_OK;
	}
	return FUNC_INPUT_ERR;
}

