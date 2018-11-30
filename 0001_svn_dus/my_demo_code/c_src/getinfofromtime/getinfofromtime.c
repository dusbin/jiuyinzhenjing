#include<stdio.h>
#include "getinfofromtime.h"
unsigned int getinfo(int flag,unsigned int date){
	if(flag == GET_NONE){
		return date;
	}
	if(flag == GET_TIME){
		date = date % 86400;
	}
	if(flag == GET_WEEK){
		date = date /86400;
		date = date%7;
	}
	if(flag == GET_HOUR){
		date = date/3600;
		date = date%24;
		
	}
	if(flag == GET_SEC){
		date = date/60;
		date = date%60;
	}
	if(flag == GET_MIN){
		date = date%60;
	}
	//date = time(NULL);
	return date;
}
int main(){
	printf("getinfo(GET_NONE) = %ld\n",getinfo(GET_NONE,time(NULL)));
	printf("getinfo(GET_TIME) = %ld\n",getinfo(GET_TIME,time(NULL)));
	printf("getinfo(GET_WEEK) = %ld\n",getinfo(GET_WEEK,time(NULL)));
	printf("getinfo(GET_HOUR) = %ld\n",getinfo(GET_HOUR,time(NULL)));
	printf("getinfo(GET_SEC) = %ld\n",getinfo(GET_SEC,time(NULL)));
	printf("getinfo(GET_MIN) = %ld\n",getinfo(GET_MIN,time(NULL)));
	return 0;
}