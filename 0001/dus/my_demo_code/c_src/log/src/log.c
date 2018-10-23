#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "log.h"
time_t *timep = NULL;
FILE *g_log;
char* gettime(){
	if(timep == NULL){
		timep = malloc(sizeof(time_t));
	}
	time(timep);
	char *s = ctime(timep);
	s[strlen(s)-1] = '\0';
	return s;
}
int init_log(){
	if(g_log != NULL){
		return 0;
	}
	g_log=fopen("/opt/log.txt","a+");
	if(g_log == NULL){
		printf("open log file failed!\n");
		return -1;
	}
}
int release_log(){
	fclose(g_log);
	g_log = NULL;
	return 0;
}

