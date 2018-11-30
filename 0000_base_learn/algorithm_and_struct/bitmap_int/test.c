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
int a[32];//申请内存的大小
void test_01(){
	for(int i =0;i<32;i++){
		a[i] = 0;
	}
	int value = 0;
	int row = 0,col = 0,flg = 0;
	for(int i=0;i<1023;i++){
		value = rand()%1023;
		//scanf("%d",&value);
		row = value/32;
		col = value%32;
		flg = 1<<col;
		if((a[row]&(flg)) != (flg)){
			a[row] = a[row] + flg;
			printf("add %d a[%d] = %d\n",value,row,a[row]);
		}
	}
	printf("print\n");
	int count = 0;
	for(int i=0;i<1024;i++){
		row = i/32;
		col = i%32;
		flg = 1<<col;
		if((a[row]&(flg)) == (flg)){
			printf("has %d\n",i);
			count++;
		}
	}
	printf("count = %d\n",count);
}
int main(){
	test_01();
	return 0;
}


