#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
void DataWrite(){
	//准备数据
	double pos[200];
	int i = 0;
	for(i = 0; i < 200; i ++ ){
		if(i%2 == 0){
			pos[i] = i +1;
		}else{
			pos[i] = i * 2;
		}
	}
	for(i = 0; i < 200; i ++ ){
		printf("%lf\n",pos[i]);
		
	}
	//写出数据
	FILE *fid;
	fid = fopen("binary.dat","wb");
	if(fid == NULL)
	{
		printf("写出文件出错");
		return;
	}
	int mode = 1;
	printf("mode为1，逐个写入；mode为2，逐行写入\n");
	scanf("%d",&mode);
	if(1==mode)
	{
		for(i = 0; i < 200; i++)
			fwrite(&pos[i],sizeof(double),1,fid);
	}
	else if(2 == mode)
	{
		fwrite(pos, sizeof(double), 200, fid);
	}
	fclose(fid);
/*
--------------------- 
作者：nichengwuxiao 
来源：CSDN 
原文：https://blog.csdn.net/nichengwuxiao/article/details/78789225?utm_source=copy 
版权声明：本文为博主原创文章，转载请附上博文链接！
*/
}
int main(int argc,char *argv[]){
	printf("begin\n");
	DataWrite();
	printf("end\n");
	return 0;
}

