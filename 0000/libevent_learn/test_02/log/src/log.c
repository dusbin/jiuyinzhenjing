#include <stdio.h>
#include <time.h>
#include <event2/event.h>
#include <string.h>//sleep
#include <unistd.h>
#include <stdlib.h>
#include "../inc/log.h"

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

void write_to_file_cb(int severity,const char *msg){
	const char *s;
	if(!logfile)
		return;
	switch(severity){
		case _EVENT_LOG_DEBUG	:	s=DEBUG_COLOUR"DEBUG"NONE;		break;
		case _EVENT_LOG_MSG		:	s=INFO_COLOUR"INFO "NONE;			break;
		case _EVENT_LOG_WARN	:	s=WARNING_COLOUR"WARN "NONE;	break;
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
char* gettime(){
	if(timep == NULL){
		timep = malloc(sizeof(time_t));
	}
	time(timep);
	char *s = ctime(timep);
	s[strlen(s)-1] = '\0';
	return s;
}
void init_log(const char *file){
	fp = fopen(file,"w+");
	set_logfile(fp);
}
void release_log(){
	fclose(fp);
	fp = NULL;
	free(timep);
	timep = NULL;
}

