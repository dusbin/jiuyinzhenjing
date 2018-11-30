#ifndef __COMM_DEFINE_H__
#define __COMM_DEFINE_H__
#include "simple_list.h"

/*gloable value */
struct list_head g_plugin_list;
struct list_head g_memory_list;
/*flag enum*/
enum{
	unknown	= 		0,
	unlock	= 		1,
	lock	= 		2,
};

/*char value[] len*/
#define FILE_LEN	128
#define MD5_LEN		33
#define CMD_LEN		1024
#define MAX_TIME	100
#define BUFF_LINE	1024
#define LIB_FLAG_LEN 33

/*path and name*/
#define LIBS_PATH	"/opt/plugin/libs/"

/*FUNC code*/
#define FUNC_OK		0
#define FUNC_ERR	-1

#endif

