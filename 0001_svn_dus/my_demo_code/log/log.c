#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
time_t *timep = NULL;
char* gettime(){
	if(timep == NULL){
		timep = malloc(sizeof(time_t));
	}
	time(timep);
	char *s = ctime(timep);
	s[strlen(s)-1] = '\0';
	return s;
}
#define __DEBUG__
//#undef __DEBUG__ //屏蔽此行 放开调试
#ifdef __DEBUG__
#define DEBUG(formnt,...) \
    fprintf(g_log,"\n\033[31m ERROR \033[0m[%s],filename:[%s],function:[%s()] line:[%d] "formnt, \
            gettime(),__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__)
#else
#define DEBUG(formnt,...)
#endif

#define __INFO__
//#undef __INFO__ //屏蔽此行 放开调试
#ifdef __INFO__
#define INFO(format,...) \
    fprintf(g_log,"\n\033[31m INFO  \033[0m[%s],filename:[%s], function: [%s()], line :[%d]"format, \
            gettime(),__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__)
#else
#define INFO(format,...)
#endif

FILE *g_log;
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
	g_log=fopen("/opt/log.txt","a+");
	if(g_log == NULL){
		printf("open log file failed!\n");
		return -1;
	}
	DEBUG("begin");
	INFO("tel = %s","18792741184");
	ret = create_list();
	DEBUG("end");
	fclose(g_log);
	g_log = NULL;
	return 0;
}



















