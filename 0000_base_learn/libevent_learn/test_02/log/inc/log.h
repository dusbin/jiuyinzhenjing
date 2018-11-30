#ifndef __LOG_H__
#define __LOG_H__
#define EVENT_LOG_DEBUG 0
#define EVENT_LOG_MSG	1
#define EVENT_LOG_WARN	2
#define EVENT_LOG_ERR	3
#define EVENT_LOG_TRACE 4
#define EVENT_LOG_NONE	5

static FILE *logfile = NULL;
char* gettime();
void set_logfile(FILE *f);
static time_t *timep = NULL;
static FILE *fp = NULL;
void write_to_file_cb(int severity,const char *msg);
#define LOG(flag,format,...)\
{\
	{\
		char msg_data[2048] = {0};\
		sprintf(msg_data,"[%s][%s][%s()][%d]:"format,gettime(),__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__);\
		write_to_file_cb(flag,msg_data);\
	}\
}
void init_log(const char *file);
void release_log();
#endif

