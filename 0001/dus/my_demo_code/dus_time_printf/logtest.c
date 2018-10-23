#include <stdio.h>
#include <time.h>
int main(){
	printf("begin\n");
	time_t tv;
	time(&tv);
	struct tm *systime = NULL;
	char timestr[30]={0};
	systime = (struct tm *)localtime(&tv);
	strftime(timestr,30,"_%Y_%m_%d_%H_%M_%S",systime);
	printf("timestr = [%s]\n",timestr);
	printf("end\n");
	return 0;
}

