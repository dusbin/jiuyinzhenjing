#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(int argc,char**argv){
	//if(0==strcmp(argv[0],"dus")){
	if(NULL!=strstr(argv[0],"dus")){
		printf("you use [%s],thx!\n",argv[0]);
	//}else if(0==strcmp(argv[0],"uxsino")){
	}else if(NULL!=strstr(argv[0],"uxsino")){
		printf("you use [%s],sorry!\n",argv[0]);
	}else{
		printf("[%s] is undefined!\n",argv[0]);
	}
	return 0;
}