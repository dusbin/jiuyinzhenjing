/**
* @file CommonOperator.h
* 创建日期：2017/09/25
* 创建作者：shiqing
* 修改履历：001)[修改原因1]
* 修改作者：001)修改者 YYYY/MM/DD
* All Right Reserved, Copyright(c) Beijing Uxsino Limited 2017.
* **************************************************************/
#ifndef __COMMON_OPERATOR_H__
#define __COMMON_OPERATOR_H__

#include "Common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef FILE uxsino_file;
typedef struct flock uxsino_file_lock;

/**
 * @def offsetof
 * @brief 计算指定类型中成员在此类型中的偏移量
 */
#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

#define sock_fd_list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define sock_recv_buf_list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define sock_send_buf_list_entry(ptr, type, member) \
	container_of(ptr, type, member)

/**
 * @def container_of
 * @brief 通过struct的成员变量获取struct对象
 * @ptr: struct成员变量指针
 * @type: struct的类型
 * @member:	ptr在struct中的名称
 */
#ifndef container_of
#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})
#endif

#ifndef RET_AND_RETURN
#define RET_AND_RETURN(ret) \
    do{                     \
        if ((ret) < 0) return -1;     \
    }while(0)
#endif

/**
 * @brief  申请内存
 */
#define uxsino_malloc(type, size, sign) \
    ((type*)malloc((size)))

/**
 * @brief  释放内存
 */
#define uxsino_free(ptr)    \
    do{if((ptr)){free((ptr));(ptr) = NULL;}}while(0)

/**
 * @brief  打开文件
 */
#define uxsino_fopen(filename, mode, fileauth)  \
    (fopen((filename), (mode)))
/**
 * @brief   关闭文件
 */
#define uxsino_fclose(fp) \
    do {    \
        fclose(fp);   \
    }while(0)
/**
 * @brief    重定位文件
 */
#define uxsino_fseek(fp, offset, whence)  \
    (fseek((fp), (offset), (whence)))
/**
 * @brief   读文件
 */
#define uxsino_fread(buf, size, count, fp)  \
    (fread((buf), (size), (count), (fp)))
/**
 * @brief   写文件
 */
#define uxsino_fwrite(buf, size, count, fp) \
   (fwrite((buf), (size), (count), (fp)))

#define uxsino_fcntl(fp, mode, arg)  \
    (fcntl((fp), (mode), (arg)))

#define uxsino_fflush(fp) \
    (fflush((fp)))

#define uxsino_ftell(fp)	\
	(ftell(fp))

#define __GFP_WAIT  0x10u
#define __GFP_IO       0x40u
#define __GFP_FS       0x80u
#define GFP_KERNEL  (__GFP_WAIT | __GFP_IO | __GFP_FS)
#define GFP_USER 0x00



/**
 * 日志打印级别
 */

#define UX_LOG printf		/* 以前版本的打印日志宏，以后不再调用 */

#define LOG_DEBUG "DEBUG"		/* 调试日志打印级别 */
#define LOG_TRACE "TRACE"		/* 跟踪日志打印级别 */

#define LOG_INFO  "INFOR"		/* 运行信息日志打印级别 */
#define LOG_WARN  "WARNING"		/* 告警日志打印级别 */
#define LOG_ERRO "ERROR"		/* 错误日志打印级别 */
#define LOG_FATA  "FATAL"		/* 致命问题日志打印级别 */


/* 打印日志级别，可以在运行时动态改变 */
#define LOG_DEBUG_LEVEL 0x20		/* 调试日志打印级别 */
#define LOG_TRACE_LEVEL 0x10		/* 跟踪日志打印级别 */

#define LOG_INFO_LEVEL  0x08		/* 运行信息日志打印级别 */
#define LOG_WARN_LEVEL  0x04    	/* 告警日志打印级别 */
#define LOG_ERRO_LEVEL  0x02		/* 错误日志打印级别 */
#define LOG_FATA_LEVEL  0x01		/* 致命问题日志打印级别 */

#define LOG_ALL_LEVEL	0x3F

#ifdef _GUN_LINUX_
#define LOG(level, format, ...) \
    do { \
        fprintf(stderr, "%s:[%s:%s:%d] " format "\n", \
            level, __FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__ ); \
    } while (0)

#define PROMPT(level, format, ...) \
    do { \
        fprintf(stderr, "%s: " format "\n", \
            level, ##__VA_ARGS__ ); \
    } while (0)

#else
#define LOG(level, format, ...) \
    do { \
        printf("%s:[%s:%s:%d] " format "\n", \
            level, __FUNCTION__, __FILE__, __LINE__, ##__VA_ARGS__ ); \
    } while (0)

#define PROMPT(level, format, ...) \
    do { \
    	printf(stderr, "%s: " format "\n", \
            level, ##__VA_ARGS__ ); \
    } while (0)

#endif

/* 调用以下打印日志的宏 */
extern unsigned short g_log_print_level_switch; /* define in main/src/Main.c */

#define DEBUG_LOG(format, ...) do { if(g_log_print_level_switch&LOG_DEBUG_LEVEL) LOG(LOG_DEBUG, format, __VA_ARGS__); }while(0)
#define TRACE_LOG(format, ...) do { if(g_log_print_level_switch&LOG_TRACE_LEVEL) LOG(LOG_TRACE, format, __VA_ARGS__); }while(0)

#define INFO_LOG(format, ...) do { if(g_log_print_level_switch&LOG_INFO_LEVEL) LOG(LOG_INFO, format, __VA_ARGS__); }while(0)
#define WARN_LOG(format, ...) do { if(g_log_print_level_switch&LOG_WARN_LEVEL) LOG(LOG_WARN, format, __VA_ARGS__); }while(0)
#define ERR_LOG(format, ...)  do { if(g_log_print_level_switch&LOG_ERRO_LEVEL) LOG(LOG_ERRO, format, __VA_ARGS__); }while(0)
#define FATA_LOG(format, ...) do { if(g_log_print_level_switch&LOG_FATA_LEVEL) LOG(LOG_FATA, format, __VA_ARGS__); }while(0)


#endif // __COMMON_OPERATOR_H
