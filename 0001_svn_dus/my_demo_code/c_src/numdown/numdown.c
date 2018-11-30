//https://baike.baidu.com/item/%E5%88%86%E5%BD%A2%E5%9B%BE/8662402?fr=aladdin
//https://baike.baidu.com/item/%E5%86%B0%E9%9B%B9%E7%8C%9C%E6%83%B3/659469?fr=aladdin
#include<stdio.h>
#include <unistd.h>
#define PATH "/opt/numdown"
#define PATH_LEN 256
#define CMD_LEN 512
#define FILENAME 1
#define PATHNAME 2
double minspeed = 999;
double maxspeed = 0;
int deep = 0;
int checkpath(const char* path,int flag){
	char cmd[CMD_LEN] = {0};
	if((access(path,F_OK)) == 0){
		sprintf(cmd,"rm -rf %s",path);
		system(cmd);
	}
	if(flag == FILENAME){
		sprintf(cmd,"touch %s",path);
		system(cmd);
	}else if(flag == PATHNAME){
		sprintf(cmd,"mkdir -p  %s",path);
		system(cmd);
	}
	return 0;
}
int writenumbertofile(int number,const char* path){
	char cmd[CMD_LEN] = {0};
	sprintf(cmd,"echo %d >> %s",number,path);
	system(cmd);
	return 0;
}
int numdown(int number){
	if(number <= 0){
		return -1;
	}
	int count = 0;
	int statnumber = number;
	int biggestnumber = number;
	char cmd[CMD_LEN] = {0};
	char path[PATH_LEN] = {0};
	//sprintf(path,"%s/%d",PATH,number);
	sprintf(path,"%s/%d",PATH,1);
	//checkpath(path,FILENAME);
	//sprintf(cmd,"echo downnum[%d] >> %s",number,path);
	//system(cmd);
BEGIN:
	if(number == 1){
		//writenumbertofile(number,path);
		count++;
		//sprintf(cmd,"echo \"count[%d]&biggestnumber[%d]\" >> %s",,count,biggestnumber,path);
		sprintf(cmd,"echo \"number[%d]count[%d]&biggestnumber[%d]speed[%f]\" >> %s",statnumber,count,biggestnumber,(double)statnumber/(double)count,path);
		system(cmd);
		if((double)statnumber/(double)count < minspeed){
			sprintf(cmd,"echo \"number[%d]count[%d]&biggestnumber[%d]speed[%f]\" >> %s/min",statnumber,count,biggestnumber,(double)statnumber/(double)count,PATH);
			system(cmd);
			minspeed = (double)statnumber/(double)count;
		}
		if(maxspeed < (double)statnumber/(double)count){
			sprintf(cmd,"echo \"number[%d]count[%d]&biggestnumber[%d]speed[%f]\" >> %s/max",statnumber,count,biggestnumber,(double)statnumber/(double)count,PATH);
			system(cmd);
			maxspeed = (double)statnumber/(double)count;
		}
		if(count > deep){
			sprintf(cmd,"echo \"number[%d]count[%d]&biggestnumber[%d]speed[%f]\" >> %s/deep",statnumber,count,biggestnumber,(double)statnumber/(double)count,PATH);
			system(cmd);
			deep = count;
		}
		return 0;
	}else if(number%2 == 0){
		//writenumbertofile(number,path);
		number = number/2;
		if(biggestnumber < number){
			biggestnumber = number;
		}
		count++;
		goto BEGIN;
	}else{
		//writenumbertofile(number,path);
		number = number*3 + 1;
		if(biggestnumber < number){
			biggestnumber = number;
		}
		count++;
		goto BEGIN;
	}
}
int main(){
	char cmd[CMD_LEN] = {0};
	printf("begin\n");
	checkpath(PATH,PATHNAME);
	int i = 0;
	for(i = 1;i <= 10000000;i++){
		numdown(i);
		printf("%d\r",i);
		fflush(stdout);
	}
	//numdown(81);
	//numdown(243);
	printf("end\n");
	return 0;
}