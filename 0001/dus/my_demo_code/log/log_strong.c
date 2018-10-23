#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define __DEBUG__
//#undef __DEBUG__ //屏蔽此行 放开调试
#ifdef __DEBUG__
#define DEBUG(format,...) \
	fprintf(g_log,"[%s][%s][%d]"format"\n",__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__,"\n") 
#else
#define DEBUG(format,...)
#endif
FILE *g_log;
int create_list()
{
	DEBUG("begin");
	printf("exec create_list\n");
	DEBUG("end");
	return 0;
}
int main(int ragc,char * argv[])
{
	int ret = 0;
	g_log=fopen("/opt/log.txt","a+");
	if(g_log == NULL){
		printf("open log file failed!\n");
		return -1;
	}
	DEBUG("begin");
	DEBUG("tel = %s","18792741184");
	ret = create_list();
	DEBUG("end");
	fclose(g_log);
	g_log = NULL;
	return 0;
}



















