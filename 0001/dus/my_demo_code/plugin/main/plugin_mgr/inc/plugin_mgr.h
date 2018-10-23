#ifndef __PLUGIN_MGR_H__
#define __PLUGIN_MGR_H__
#include "simple_list.h"
#include "comm_define.h"

typedef struct plugin_unit{
	struct list_head element;
	char plugin_name[FILE_LEN];
	char plugin_md5[MD5_LEN];
	char plugin_mod[LIB_FLAG_LEN];
	int flag;
}plugin_unit;
char* get_plugin_name_by_mod(const char* mod);
int monitor_libs();
void print_plugin();//test 打印插件信息
int monitor_libs_test(); //test 查看g_plugin_list的值

#endif

