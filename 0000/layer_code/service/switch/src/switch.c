#include "switch.h"
#include "gettime.h"
#include "getuser.h"
#include "getversion.h"
#include<string.h>
int switch_func(char* input,char* output){
	int len = strlen(input);
	memset(output,0x00,sizeof(output));
	if(len <= 0){
		strcpy(output,"-1");
		return 0;
	}
	switch(input[0]){
		case '1':get_time(output);break;
		case '2':get_user(output);break;
		case '3':get_version(output);break;
		default:
			strcpy(output,"0");	
	}
	return 0;
}