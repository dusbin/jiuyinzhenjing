#include<stdio.h>
int main(){
	printf("begin\n");
	extern char **environ;
	int i;
	for(i = 0;environ[i] != NULL;i++)
		printf("env : [%s]\n",environ[i]);
	printf("end\n");
	return 0;
}

