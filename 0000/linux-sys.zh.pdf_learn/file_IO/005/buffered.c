//https://www.cnblogs.com/orlion/p/6258691.html
// $./buffered
//	 input:1234567
//   output:1234
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(void){
	char buf[5];
	FILE *myfile = stdin;
	fgets(buf,5,myfile);
	fputs(buf,myfile);
	//buf[5] = '\0';
	printf("%s",buf);
	return 0;
}

