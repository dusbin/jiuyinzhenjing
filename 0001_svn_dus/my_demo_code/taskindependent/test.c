#include <stdio.h>
#include <string.h>
#include "cJSON.h"
#include <stdlib.h>
#include "test.h"
int get_config()
{
	int state = 0;
	FILE *fp = NULL;	
	long len = 0;	
	char *data = NULL;
	cJSON *json = NULL;
	int i = 0;
	int size = 0;

	memset(&g_uxcfg,0,sizeof(g_uxcfg));
	fp=fopen("cfg.json","rb");
	if(NULL == fp){
		return -1;
	}
	fseek(fp,0,SEEK_END);	
	len=ftell(fp);	
	fseek(fp,0,SEEK_SET);	
	data=(char*)malloc(len+1);
	if(NULL == data){
		fclose(fp);
		return -1;
	}
	memset(data,0,len+1);	
	fread(data,1,len,fp);	
	fclose(fp);	
	
	printf("deta==%s\n",data);
	// 解析数据包
	json = cJSON_Parse(data);
	if(NULL == json || cJSON_Array != json->type){
		state = -1;
		printf(("jp_cfg_loadconfig cJSON_Parse failed\n"));
		goto END;
	}
		
	size = cJSON_GetArraySize(json);
	printf("size==%d\n",size);
	for(i=0;i<size && i<MAX;i++)
	{
		int j = 0;
		cJSON * json_id = NULL;
		cJSON * json_moudles = NULL;
		cJSON * cj = NULL;
		struct mould *moulds  = &g_uxcfg.mou[i];
		cj = cJSON_GetArrayItem(json,i);
		if (cj->type != cJSON_Object){
			continue;
		}
		json_id = cJSON_GetObjectItem( cj , "id");
		moulds->id = json_id->valueint;
		printf("moulds->id==%d\n",moulds->id);
		 
		json_moudles = cJSON_GetObjectItem( cj , "moudles");
		strcpy(moulds->path,json_moudles->valuestring);
		printf("json_moudles->valuestring==%s\n",json_moudles->valuestring);
		g_uxcfg.num++;
	}
	printf("g_uxcfg.num==%d\n",g_uxcfg.num);
	state = 0;
END:
	if(NULL != data){
		free(data);
		data = NULL;
	}
	if(NULL != json){
		cJSON_Delete(json);
		json = NULL;
	}
	return state;
}
int jp_moudel_load()
{
	int i=0;
	int a =6,b=2;
	for(i=0;i<g_uxcfg.num;i++)
	{
		void *handle;
		handle = dlopen(g_uxcfg.mou[i].path, RTLD_LAZY);
		if (!handle) {
			printf("load %s error!\n",g_uxcfg.mou[i].path);
		}
		g_uxcfg.mou[i].moudelhand=handle;
		*(void **) (&cac_func) = dlsym(handle,"fun");
		printf("a==%d,b==%d,result====%d mou.moudelhand=%ld\n",a,b,(*cac_func)(a,b),(unsigned long*)g_uxcfg.mou[i].moudelhand);
	}
	return 0;
}

int jp_moudel_unload()
{
	int i=0;
	printf("uxcfg.num=%d\n",g_uxcfg.num);
	for(i=0;i<g_uxcfg.num;i++)
	{
		if(NULL != g_uxcfg.mou[i].moudelhand)
		{
			printf("moudelhand==%ld\n",(unsigned long*)g_uxcfg.mou[i].moudelhand);
			dlclose(g_uxcfg.mou[i].moudelhand);
			g_uxcfg.mou[i].moudelhand=NULL;
		}
	}
	return 0;
}

int main()//1+ 2- 3* 4/
{
	get_config();
	jp_moudel_load();
	jp_moudel_unload();
}


















