#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<string.h>
typedef struct {
    char c;
    int h;
    short n;
    long m;
    float f;
    double d1;
    char *s;
    double d2;
}st;
void structWrite(){
    FILE *fp;
    st sa,sb;
    char *str="abcdefg";
    sa.c='K';
    sa.h=-3;
    sa.n=20;
    sa.m=100000000;
    sa.f=33.32f;
    sa.d1=78.572;
    sa.d2=33.637;
    sa.s=str;
    fp=fopen("st.txt","w+");
    if(!fp)
    {
        printf("errror!\n");
        exit(-1);
    }
    printf("sa:c=%c,h=%d,n=%d,m=%d,f=%f,d1=%f,s=%s,d2=%f\n",sa.c,sa.h,sa.n,sa.m,sa.f,sa.d1,sa.s,sa.d2);
    printf("sizeof(sa)=%d:&c=%x,&h=%x,&n=%x,&m=%x,&f=%x,&d1=%x,&s=%x,&d2=%x\n",sizeof(sa),&sa.c,&sa.h,&sa.n,&sa.m,&sa.f,&sa.d1,&sa.s,&sa.d2);
    fwrite(&sa,sizeof(sa),1,fp);
    rewind(fp);
    fread(&sb,sizeof(sb),1,fp);
    printf("sb:c=%c,h=%d,n=%d,m=%d,f=%f,d1=%f,s=%s,d2=%f\n",sb.c,sb.h,sb.n,sb.m,sb.f,sb.d1,sb.s,sb.d2);
       
    fclose(fp);
}
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
	//DataWrite();
	structWrite();
	printf("end\n");
	return 0;
}

