#include "comm_define.h"
#include "uxmd5.h"
#include "simple_list.h"
#include "plugin_mgr.h"
#include <dlfcn.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
char* get_plugin_name_by_mod(const char* mod){
	if(list_empty(&g_plugin_list)){
		return "unknown";
	}
	struct list_head* pos = NULL,*n = NULL;
	plugin_unit* plugin_unit_tmp = NULL;
	list_for_each_safe(pos,n, &g_plugin_list)
    {
        plugin_unit_tmp = container_of(pos, plugin_unit, element);
		if(0==strcmp(plugin_unit_tmp->plugin_mod,mod)){
		printf("get_plugin_name_by_mod plugin_unit_tmp->plugin_mod(%s),mod(%s)\n",plugin_unit_tmp->plugin_mod,mod);
            return plugin_unit_tmp->plugin_name;
        }
    }
	return "unknown";
}
del_list(){
	if(list_empty(&g_plugin_list)){
		return FUNC_OK;
	}
	struct list_head* pos = NULL,*n = NULL;
	plugin_unit* plugin_unit_tmp = NULL;
	list_for_each_safe(pos,n, &g_plugin_list)
    {
        plugin_unit_tmp = container_of(pos, plugin_unit, element);
        if(plugin_unit_tmp->flag == unlock){
            list_del(pos);
			free(plugin_unit_tmp);
			plugin_unit_tmp = NULL;
        }else{
            plugin_unit_tmp->flag =unlock;
        }
    }
    return FUNC_OK;
}
int find_plugin_from_list(char lib_name[], char lib_md5[], int flag){
	if(list_empty(&g_plugin_list)){
		return FUNC_OK;
	}
	struct list_head* pos = NULL,*n = NULL;
	plugin_unit* plugin_unit_tmp = NULL;
	list_for_each_safe(pos,n, &g_plugin_list)
    {
        plugin_unit_tmp = container_of(pos, plugin_unit, element);
		if((0==strcmp(plugin_unit_tmp->plugin_name,lib_name))&&(flag == plugin_unit_tmp->flag)){
			//printf("is in list\n");
			list_del(pos);
			free(plugin_unit_tmp);
			plugin_unit_tmp = NULL;
			plugin_unit* plugin_add = (plugin_unit*)malloc(sizeof(plugin_unit));
			plugin_add->flag = flag;
			strcpy(plugin_add->plugin_name,lib_name);
			strcpy(plugin_add->plugin_md5,lib_md5);
			list_add_tail(&plugin_add->element,&(g_plugin_list));
			return FUNC_ERR;		
		}
    }
	//printf("is not in list\n");
    return FUNC_OK;
}
typedef int (*CAC_FUNC)(char *);
static void *thrd_monitor_libs(){
	FILE *fp = NULL;
	char lib_name[FILE_LEN] = {0};
	char cmd[CMD_LEN] = {0};
	int lib_name_len = -1;
	int ret = FUNC_ERR;
	char lib_md5[MD5_LEN] = {0};
	INIT_LIST_HEAD(&g_plugin_list);
	void *handle;
	char *error;
	CAC_FUNC cac_func = NULL;
	memset(cmd,0x00,sizeof(cmd));
	sprintf(cmd,"find %s -name lib*.so",LIBS_PATH);
	for(;;){
		if(NULL ==(fp = popen(cmd,"r"))){
			goto WARNING;
		}
		//printf("-------------------------\n");
		memset(lib_name,0x00,sizeof(lib_name));
		memset(lib_md5,0x00,sizeof(lib_md5));
		while(NULL!=fgets(lib_name,sizeof(lib_name),fp)){
			lib_name_len = strlen(lib_name);
			if(lib_name_len > 0){
				lib_name[lib_name_len-1] = '\0';
			}
			lib_name_len = -1;
			ux_md5sum(lib_name,lib_md5);
			//printf("libname[%s] is [%s]\n",lib_name,lib_md5);
			if(FUNC_OK == find_plugin_from_list(lib_name,lib_md5,lock)){
				//printf("add plugin info\n");
				plugin_unit* plugin_add = (plugin_unit*)malloc(sizeof(plugin_unit));
				plugin_add->flag = lock;
				strcpy(plugin_add->plugin_name,lib_name);
				strcpy(plugin_add->plugin_md5,lib_md5);
				handle = dlopen(lib_name, RTLD_LAZY);
				if (!handle) {
					printf("load %s error!\n",lib_name);
					goto WARNING;
				}
				dlerror(); 
				//获取函数
				*(void **) (&cac_func) = dlsym(handle, "getmod");
				ret = (*cac_func)(plugin_add->plugin_mod);
				list_add_tail(&plugin_add->element,&(g_plugin_list));
			}
			memset(lib_md5,0x00,sizeof(lib_md5));
			memset(lib_name,0x00,sizeof(lib_name));
		}
		del_list(&g_plugin_list);
		pclose(fp);
		fp = NULL;
	WARNING:
		sleep(10);//每10秒更新一次
		if (handle)
			dlclose(handle);
	}
	pthread_detach(pthread_self());
	pthread_exit(NULL);
	return NULL;
}
int monitor_libs(){
	pthread_t thrd_id;
	pthread_create(&thrd_id,NULL,thrd_monitor_libs,NULL);
	return FUNC_OK;
}
/*test code*/
void print_plugin(){
	printf("--------begin printf--------\n");
	if(list_empty(&g_plugin_list)){
		printf("list_empty\n");
		printf("---------end printf---------\n");
		return;
	}
	struct list_head* pos = NULL,*n = NULL;
	plugin_unit* plugin_unit_tmp = NULL;
	list_for_each_safe(pos,n, &g_plugin_list)
    {
        plugin_unit_tmp = container_of(pos, plugin_unit, element);
		printf("plugin name :[%s],plugin md5 :[%s],plugin flag : [%d],plugin mod : [%s]\n",plugin_unit_tmp->plugin_name,plugin_unit_tmp->plugin_md5,plugin_unit_tmp->flag,plugin_unit_tmp->plugin_mod);
    }
	printf("---------end printf---------\n");
	return;
}
static void *thrd_monitor_libs_test(){
	for(;;){
		print_plugin();
		sleep(2);
	}
	pthread_detach(pthread_self());
	pthread_exit(NULL);
	return NULL;
}
int monitor_libs_test(){
	pthread_t thrd_id;
	pthread_create(&thrd_id,NULL,thrd_monitor_libs_test,NULL);
	return FUNC_OK;
}

