#include<stdio.h>
#include "log.h"
int create_list()
{
	DEBUG("begin");
	printf("exec create_list\n");
	INFO("end");
	for(;;){
		DEBUG("123");
		printf("123");
		sleep(1);
	}
	return 0;
}
int main(int ragc,char * argv[])
{
	int ret = 0;
	init_log();
	printf("1\n");
	printf("%d\n",g_log);
	DEBUG("begin");
	printf("%d\n",g_log);
	INFO("tel = %s","18792741184");
	ret = create_list();
	DEBUG("end");
	release_log();
	return 0;
}