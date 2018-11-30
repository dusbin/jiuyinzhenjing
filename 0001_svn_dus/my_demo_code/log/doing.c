#include<stdio.h>
#include<string.h>
#include<unistd.h>
#define LEN 20
void proc_1(){
	int i = 0;
	printf("one\n");
	for(i = 0;i < 100;++i){
		printf("-");
	}
	fflush(stdout);
	for(i = 0;i < 100;++i){
		printf("\b");
	}
	for(i = 0;i < 100;++i){
		printf(">");
		fflush(stdout);
		sleep(1);
	}
	printf("\n");
}
void proc_2(){
	printf("two\n");
	char buf[103];
	memset(buf,' ',sizeof(buf));
	buf[0]='[';
	buf[101]=']';
	buf[102]='\0';
	int i = 1;
	char index[6] = "-\\|/\0";
	while(i<=100){
		buf[i]='=';
		if(i == 100){
			printf("%s [%d%%]      ",buf,i);
		}else{
			printf("%s [%d%%][%c]\r",buf,i,index[i%4]);
		}
		fflush(stdout);
		sleep(1);
		i++;
	}
	printf("\n");
}
void proc_3(){
	printf("three\n");
	char buf[LEN];
	memset(buf,' ',sizeof(buf));
	int i = 0;
	int flag = 0;
	int all = 0;
	int flag1=0;
	while(i<100){
		memset(buf+1,' ',(LEN-3));
		all = LEN-6;
		buf[0]='[';
		buf[LEN-2]=']';
		buf[LEN-1]='\0';
		flag=i%all;
		flag1 = (i/all)%2;
		if(flag1 == 0){
			buf[flag+1]='<';buf[flag+2]='=';buf[flag+3]='>';
		}else{
			buf[all-flag+1]='<';buf[all-flag+2]='=';buf[all-flag+3]='>';
		}
		printf("%s\r",buf);
		fflush(stdout);
		sleep(1);
		i++;
	}
	printf("\n");
}
int main(){
	proc_1();
	proc_2();
	proc_3();
	return 0;
}

