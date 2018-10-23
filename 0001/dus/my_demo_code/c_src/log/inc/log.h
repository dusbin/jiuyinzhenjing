#ifndef __LOG_H__
#define __LOG_H__
extern FILE *g_log = NULL;
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
int init_log();
int release_log();
#endif