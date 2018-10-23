#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>
#include "plugin_mgr.h"
#include "simple_list.h"
#include "secure_memory.h"

//函数指针
typedef int (*CAC_FUNC)(int, int);
typedef int (*EXEC_FUNC)(char *,int, char *,int);
int main(){
    void *handle;
    char *error;
    CAC_FUNC cac_func = NULL;
	EXEC_FUNC exec_func = NULL;
	char lib_name[FILE_LEN] = {0};
    int a,b;
    char c[LIB_FLAG_LEN];
	int opt_flag;
	int exit_flag = 1;
	monitor_libs();
	//monitor_libs_test();
	monitor_memory();
	//memory_test();
	do{
		printf("input:\n");
		scanf("%d %s %d",&a,c,&b);
		//scanf("%s",c);
		//scanf("%d",&b);
		printf("%d %s %d\n",a,c,b);
		memset(lib_name,0x00,sizeof(lib_name));
		printf("c is [%s]\n",c);
		strcpy(lib_name,get_plugin_name_by_mod(c));
		printf("lib_name : [%s]\n",lib_name);
		if(0 == strcmp(lib_name,"unknown")){
			memset(lib_name,0x00,sizeof(lib_name));
			goto END;
		}
		handle = dlopen(lib_name, RTLD_LAZY);
		if(exit_flag != 0){
			if (!handle) {
				printf("load %s error!\n",lib_name);
				goto WARNING;
			}
			
			//清除之前存在的错误		
			dlerror(); 
			
			//获取函数
			if(0==strcmp(c,"a")){
				*(void **) (&exec_func) = dlsym(handle, "exec");
				char *input = NULL;
				char *output = NULL;
				input = malloc(BUFF_LINE);
				output = malloc(BUFF_LINE*4);
				strcpy(input,"hello plugin");
				(*exec_func)(input,BUFF_LINE,output,BUFF_LINE*4);
				printf("output is [%s]\n",output);
			}else{
				*(void **) (&cac_func) = dlsym(handle, "func");
				printf("%d %s %d = %d\n",a,c,b,(*cac_func)(a,b));
			}
		
		WARNING:
			//关闭动态链接库
			if (handle)
				dlclose(handle);
		}
		END:
		sleep(1);
		
	}while(exit_flag != 0);
	return FUNC_OK;
}

