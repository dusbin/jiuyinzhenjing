#include <stdio.h>
#include <time.h>
#include <event2/event.h>
#include <string.h>//sleep
#include <unistd.h>
#include <stdlib.h>

#define EVENT_LOG_DEBUG 0
#define EVENT_LOG_MSG	1
#define EVENT_LOG_WARN	2
#define EVENT_LOG_ERR	3
#define EVENT_LOG_TRACE 4
#define EVENT_LOG_NONE	5

#define _EVENT_LOG_DEBUG	EVENT_LOG_DEBUG
#define _EVENT_LOG_MSG		EVENT_LOG_MSG
#define _EVENT_LOG_WARN		EVENT_LOG_WARN
#define _EVENT_LOG_ERR 		EVENT_LOG_ERR
#define _EVENT_LOG_TRACE	EVENT_LOG_TRACE
#define _EVENT_LOG_NONE		EVENT_LOG_NONE

/* colour */
#define NONE                 "\e[0m"
#define DEBUG_COLOUR         "\e[0;31m"		/* DEBUG colour */
#define INFO_COLOUR          "\e[0;36m"		/* INFO colour */
#define WARNING_COLOUR       "\e[1;33m"		/* WARNING colour */
#define ERROR_COLOUR		 "\e[1;31;40m"	/* ERROR colour */
#define TRACE_COLOUR         "\e[7m"		/* TRACE colour */

typedef void (*event_log_cb)(int severity,const char *msg);
void event_set_log_callback(event_log_cb cb);
static void discard_cb(int severity,const char *msg){
	/* This callback does nothing. */
}
static FILE *logfile = NULL;
static void write_to_file_cb(int severity,const char *msg){
	const char *s;
	if(!logfile)
		return;
	switch(severity){
		case _EVENT_LOG_DEBUG	:	s=DEBUG_COLOUR"DEBUG"NONE;		break;
		case _EVENT_LOG_MSG		:	s=INFO_COLOUR"INFO"NONE;			break;
		case _EVENT_LOG_WARN	:	s=WARNING_COLOUR"WARNING"NONE;	break;
		case _EVENT_LOG_ERR		:	s=ERROR_COLOUR"ERROR"NONE;		break;
		case _EVENT_LOG_TRACE	:	s=TRACE_COLOUR"TRACE"NONE;		break;
		default					:	s=TRACE_COLOUR"?"NONE;				break;
	}
	fprintf(logfile,"[%s] %s\n",s,msg);
}
/* Turn off all logging from Libevent. */
void suppress_logging(void){
	event_set_log_callback(discard_cb);
}
/* Redirect all Libevent log messages to the C stdio file 'f'. */
void set_logfile(FILE *f){
	logfile = f;
	event_set_log_callback(write_to_file_cb);
}
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
int main(){
	FILE *fp = fopen("/opt/test.txt","w+");
	set_logfile(fp);
	write_to_file_cb(EVENT_LOG_DEBUG,"this is debug");
	sleep(1);
	write_to_file_cb(EVENT_LOG_MSG,"this is msg");
	write_to_file_cb(EVENT_LOG_WARN,"this is warning");
	write_to_file_cb(EVENT_LOG_ERR,"this is error");
	write_to_file_cb(EVENT_LOG_TRACE,"this is trace");
	write_to_file_cb(EVENT_LOG_NONE,"this is unkown");
	fclose(fp);
	fp = NULL;
	free(timep);
	timep = NULL;
	return 0;
}
/*
编译安装libevent
	$ apt-get install git
	$ git clone https://github.com/libevent/libevent.git
	$ cd libevent
	$ apt-get install cmake
	$ apt-get install libssl-dev
	$ mkdir build && cd build
	$ cmake .. # Default to Unix Makefiles.
	$ make
	$ make verify # (optional)
	$ make install
	如果出现为找到库 需要执行
	ln -s /usr/local/lib/libevent.so.2.2.0 /usr/lib/libevent.so.2.2.0
	--------------------- 
	作者：yangzhenping 
	来源：CSDN 
	原文：https://blog.csdn.net/yangzhenping/article/details/50668445?utm_source=copy 
	版权声明：本文为博主原创文章，转载请附上博文链接！
 */

 