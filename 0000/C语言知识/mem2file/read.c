#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
void DataRead(){
	FILE *fid;
	fid = fopen("binary.dat","rb");
	if(fid == NULL)
	{
		printf("读取文件出错");
		return;
	}
	int mode = 1;
	int i = 0;
	printf("mode为1，知道pos有多少个；mode为2，不知道pos有多少个\n");
	scanf("%d",&mode);
	if(1 == mode)
	{
		double pos[200];
		fread(pos,sizeof(double),200,fid);
		for(i = 0; i < 200; i++)
			printf("%lf\n", pos[i]);
		//free(pos);
	}
	else if(2 == mode)
	{
		//获取文件大小
		fseek (fid , 0 , SEEK_END);       
		long lSize = ftell (fid);  
		rewind (fid); 
		//开辟存储空间
		int num = lSize/sizeof(double);
		double *pos = (double*) malloc (sizeof(double)*num);  
		if (pos == NULL)  
		{  
			printf("开辟空间出错");   
			return; 
		} 
		fread(pos,sizeof(double),num,fid);
		for(i = 0; i < num; i++)
			printf("%lf\n", pos[i]);
		free(pos);     //释放内存
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
int main(int argc, char *argv[])
{
	printf("begin\n");
	DataRead();
	printf("end\n");
	return 0;
}