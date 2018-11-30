#include<stdio.h>
#include<stdlib.h>
int main(){
	printf("begin\n");
	extern char **environ;
	printf("before setenv test = %s\n",getenv("dus"));
	setenv("dus","hello dus ",1);
	printf("after setenv test = %s\n",getenv("dus"));
	setenv("dus","hello dus1 ",1);//1覆盖之前的设置
	printf("after setenv test = %s\n",getenv("dus"));
	setenv("dus","hello dus2 ",0);//0不覆盖之前的设置
	printf("after setenv test = %s\n",getenv("dus"));
	unsetenv("dus");
	printf("after unsetenv test = %s\n",getenv("dus"));
	printf("end\n");
	return 0;
}

