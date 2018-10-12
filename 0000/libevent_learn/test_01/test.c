#include <stdio.h>
#include <event2/event.h>
#define EVENT_LOG_DEBUG 0
#define EVENT_LOG_MSG	1
#define EVENT_LOG_WARN	2
#define EVENT_LOG_ERR	3

#define _EVENT_LOG_DEBUG	EVENT_LOG_DEBUG
#define _EVENT_LOG_MSG		EVENT_LOG_MSG
#define _EVENT_LOG_WARN		EVENT_LOG_WARN
#define _EVENT_LOG_ERR 		EVENT_LOG_ERR

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
		case _EVENT_LOG_DEBUG	:	s="debug";	break;
		case _EVENT_LOG_MSG		:	s="msg";		break;
		case _EVENT_LOG_WARN	:	s="warn";		break;
		case _EVENT_LOG_ERR		:	s="error";	break;
		default					:	s="?";		break;
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
int main(){
	FILE *fp = fopen("/opt/test.txt","w+");
	set_logfile(fp);
	write_to_file_cb(EVENT_LOG_DEBUG,"this is debug");
	write_to_file_cb(EVENT_LOG_MSG,"this is msg");
	write_to_file_cb(EVENT_LOG_WARN,"this is warning");
	write_to_file_cb(EVENT_LOG_ERR,"this is error");
	write_to_file_cb(4,"this is unkown");
	fclose(fp);
	fp = NULL;
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

 