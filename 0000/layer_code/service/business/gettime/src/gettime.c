#include "gettime.h"
#include<string.h>
#include<stdio.h>
int get_time(char *output){
	FILE *fp = NULL;
	char buf[200];
	fp = popen("date","r");
	if(fp == NULL){
		strcpy(output,"get error");
		return 0;
	}
	fgets(buf,sizeof(buf),fp);
	pclose(fp);
	fp = NULL;
	strcpy(output,buf);
	return 0;
}