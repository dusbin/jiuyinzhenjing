#include <stdio.h>
#include <string.h>
#include "cJSON.h"
#include <stdlib.h>
void cjson_test();
int main(){
	cjson_test();
	return 0;
}
void cjson_test(){
	cJSON * root = NULL,*obj= NULL,*result=NULL;
	char *str_value = NULL;
	root = cJSON_CreateObject();
	cJSON_AddStringToObject(root, "version", "ver:0.0.0.1");
	cJSON_AddStringToObject(root, "dus", "123");
	cJSON_AddNumberToObject(root, "dus", 456);
	result = cJSON_GetObjectItem(root,"dus");
	printf("%p\n",result);
	if(result == NULL){
		printf("result is null\n");
	}
	if(result->valuestring == NULL){
		printf("result->valuestring is null\n");
	}
	printf("%s\n",result->valuestring);
	printf("%d\n",result->valueint);
	str_value = cJSON_GetObjectString(root,"version");
	if(str_value != NULL)
	printf("%s\n",str_value);
	str_value = cJSON_GetObjectString(root,"ddd");
	if(str_value != NULL)
	printf("%s\n",str_value);
	printf("-------------\n");
	result = cJSON_GetObjectItem(root,"dxx");
	printf("%p\n",result);
	if(result == NULL){
		printf("result is null\n");
	}else{
		if(result->valuestring == NULL){
			printf("result->valuestring is null\n");
			printf("%s\n",result->valuestring);
		}
	}
}