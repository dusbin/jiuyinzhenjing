#include<stdio.h>
#include<stdlib.h>
#include<string.h>
char* FileNameCopy(char* dst, char* s, int max)
{
	char* p;
	int len;
	p = s;
	len = strlen(s);
	*dst = 0;
	if (len >= max)
	{
		
		strcpy(dst,"/~/");
		p = p + (len - max + 5);
		
	}
	strcat(dst,p);
	return dst;
}
int main(){
	char dst[128] = {0};
	char s[128] = {0};
	char strde[128] = {0};
	int len = 0;
	strcpy(dst,"/opt/dzb/");
	strcpy(s,"/test/dd1");
	len = strlen(s)+1;
	printf("dst = %s,s = %s FileNameCopy(dst,s,len) = %s\n",dst,s,FileNameCopy(dst,s,len));
	return 0;
}


