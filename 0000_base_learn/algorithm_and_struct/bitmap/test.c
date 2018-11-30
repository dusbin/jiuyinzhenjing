#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define BIGGEST1 1<<32 
#define BIGGEST ((BIGGEST1) -1) //42,9496,7295 42亿

#define BITSPERWORD 32
#define SHIFT 5
#define MASK 0x1F
#define N 4000000000
int a[1 + N/BITSPERWORD];//申请内存的大小
//set 设置所在的bit位为1
void set(int i) {        
	a[i>>SHIFT] |=  (1<<(i & MASK)); 
}
//clr 初始化所有的bit位为0
void clr(int i) {        
	a[i>>SHIFT] &= ~(1<<(i & MASK)); 
}
//test 测试所在的bit为是否为1
int  test(int i){ 
	return a[i>>SHIFT] &   (1<<(i & MASK)); 
}

void test_04(){
	int i;
	for (i = 0; i < N; i++)
		clr(i);  
	while (scanf("%d", &i)&& i > 0)
		set(i);
	for (i = 0; i < N; i++)
		if (test(i))
			printf("%d\n", i);
	return;
}
static time_t *timep = NULL;
char* gettime();
void test_01();
void test_02();
void test_03();

int main(){
	//test_01();
	//test_02();
	//test_03();
	test_04();
	return 0;
}
/*功能性的问题暂时没有，存在一个*/
void test_02(){
	unsigned int size = BIGGEST/8 +1;
	char *data = (char*)malloc(size);
	memset(data,0x00,size);
	unsigned int irand = 0;
	unsigned int row = 0,col = 0;
	char *s = gettime();
	printf("begin time:%s\n",s);//begin time:Fri Oct 19 07:52:18 2018
	for(unsigned int i = 0;i < BIGGEST;i++){
		irand = rand()%BIGGEST;
		col = irand/8;
		row = irand%8;
		if((data[col]&(1<<row)) != (1<<row)){
			data[col] = data[col] + (1<<row);
		}
	}
	s = gettime();
	printf("end time:%s\n",s);//end time:Fri Oct 19 07:54:55 2018
	printf("data is already\n");
	irand = 0;
	s = gettime();
	printf("begin time:%s\n",s);
	do{
		//scanf("%u",&irand);
		col = irand/8;
		row = irand%8;
		if((data[col]&(1<<row)) != (1<<row)){
			//printf("%u 不存在！\n",irand);
		}else{
			printf("%u 存在！\n",irand);
		}
		irand++;
	}while(irand != 0 && irand <= BIGGEST);
	s = gettime();
	printf("end time:%s\n",s);
	free(data);
	data = NULL;
}
void test_03(){
	char *data = (char*)malloc(4);
	int irand = 0;
	memset(data,0x00,4);
	unsigned int row = 0,col = 0;
	for(int i = 0;i < 32;i++){
		irand = rand()%32;
		col = irand/8;
		row = irand%8;
		if((data[col]&(1<<row)) != (1<<row)){
			data[col] = data[col] + (1<<row);
			printf("添加 %d\n",irand);
		}
	}
	irand = 0;
	do{
		col = irand/8;
		row = irand%8;
		if((data[col]&(1<<row)) != (1<<row)){
			printf("%d 不存在！\n",irand);
		}else{
			printf("%d 存在！\n",irand);
		}
		irand++;
	}while(irand <= 32);
	free(data);
	data = NULL;
/* test data
添加 7
添加 6
添加 9
添加 19
添加 17
添加 31
添加 10
添加 12
添加 13
添加 26
添加 11
添加 18
添加 27
添加 3
添加 28
添加 2
添加 20
添加 24
添加 8
添加 22
添加 14
0 不存在！
1 不存在！
2 存在！
3 存在！
4 不存在！
5 不存在！
6 存在！
7 存在！
8 存在！
9 存在！
10 存在！
11 存在！
12 存在！
13 存在！
14 存在！
15 不存在！
16 不存在！
17 存在！
18 存在！
19 存在！
20 存在！
21 不存在！
22 存在！
23 不存在！
24 存在！
25 不存在！
26 存在！
27 存在！
28 存在！
29 不存在！
30 不存在！
31 存在！
32 不存在！
*/
}
void test_01(){

	unsigned int a = BIGGEST;
	printf("%u\n",a);
	char *s = gettime();
	printf("begin time:%s\n",s);//begin time:Fri Oct 19 07:06:13 2018 //ubuntu 16.04.4 memory = 4GB,CPU 4 thread
	for(unsigned int i = 0;i < BIGGEST;i++);
	s = gettime();
	printf("end time:%s\n",s);//end time:Fri Oct 19 07:06:46 2018

}
char* gettime(){
	if(timep == NULL){
		timep = (time_t*)malloc(sizeof(time_t));
	}
	time(timep);
	char *s = ctime(timep);
	s[strlen(s)-1] = '\0';
	return s;
}
