#include <stdio.h>
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
 
int main()
{	int i;
	for (i = 0; i < N; i++)
		clr(i);  
	while (scanf("%d", &i)&& i > 0)
		set(i);
	for (i = 0; i < N; i++)
		if (test(i))
			printf("%d\n", i);
	return 0;
}
/*
--------------------- 
作者：规速 
来源：CSDN 
原文：https://blog.csdn.net/hguisu/article/details/7880288?utm_source=copy 
版权声明：本文为博主原创文章，转载请附上博文链接！
*/

