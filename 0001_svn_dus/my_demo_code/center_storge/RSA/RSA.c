#include<stdio.h>
#include<stdlib.h> 
//https://blog.csdn.net/haroroda/article/details/46336053
/* 函数申明 */
int long_n(int n);
int shuru(char *arr, int k, char *wei, int is_first);
void jiami(char *arr, int k, int e, int n); 
/* 输入函数，记录从键盘输入的明文*/
int shuru(char *arr, int k, char *wei, int is_first){
    int i;
    char ch;
    /*判断是否为第一分组的输入，如果是则获取输入的字符，否则就将上一分组最后获取的字符作为这一分组的第一个字符*/
    if (is_first == 1)    
    	ch = getchar();
	else
    	ch = *wei;
    for (i = 0; (i < k) && (ch != '\n');i++){  //获取字符直到获取到回车符为止
    	arr[i] = ch;
    	ch = getchar();
    }
    *wei = ch;  //最后获取到的字符准备作为下一分组的第一个字符
    for (i = i; i < k; i++)
    	arr[i] = 'a';  //输入不够一组个数的整数倍则补'a'(即为补零)
    if (ch == '\n')  //接收到回车符返回0，否则为1
    	return 0;
    else
    	return 1;
}     
/*加密函数*/
void jiami(char *arr, int k, int e, int n)
{
    int m = 0,c=1, i, j,t=0, shu,temp,num=0;
    int *array;
    /*Mi赋值过程*/
    for (i = 0; i < k; i++){
    	temp = 1;
    	for (j = 0; j < (k-i-1)*2; j++)
    		temp = temp * 10;
    	shu = (int)arr[i] - 97;
    	m = m + temp * shu;
    }
    temp = e;
    /*获取e的二进制表达形式的位数*/
    do{
    	temp = temp / 2;
    	num++;
    } while (temp != 0);
    array = (int *)malloc(sizeof(int)*k);   //申请动态数组
    temp = e;
    /*动态数组存储e的二进制表达形式*/
    for (i = 0; i < num; i++){
    	array[i] = temp % 2;
    	temp = temp / 2;
    }
    /*避免出现天文数字的算法，详情见上文文字说明*/
    for (i = num - 1; i >= 0; i--){
    	t = t * 2;
    	temp = c*c;
    	if (temp > n){
    		for (j = 0; temp - n*j >= 0; j++);
    			j--;
    		c = temp - n*j;
    	}else
    		c = temp;
    	if (array[i] == 1){
    		t = t + 1;
    		temp = c*m;
    		if (temp > n){
    			for (j = 0; temp - n*j >= 0; j++);
    				j--;
    			c = temp - n*j;
    		}
    		else
    			c = temp;		
    	}	
    	e = e / 2;
    }
    temp = c;
    i = 0;
    /*c的位数小于分组长度则在前补零*/
    do{
    	temp = temp / 10;
    	i++;
    } while (temp != 0);
    for (i; i < num; i++)
    	printf("0");
    printf("%d", c);
}     
/*获取分组的长度*/
int long_n(int n){
    int temp,i,j,k,shi,comp=0;
    temp = n;
    /*获取n的位数*/
    for (i = 1; temp / 10 != 0; i++){
    	temp = temp / 10;
    }
    temp = i;
    /*若n的位数为基数*/
    if (i % 2 != 0){
    	i = i - 1;
    	return i;
    }else{/*若位数为偶数*/
    	for (j = 0; j < i/2; j++){
    		shi = 1;
    		for (k = 0; k < temp - 2; k++)
    			shi = shi * 10;
    		comp = comp + shi * 25;
    		temp = temp - 2;
    	}
    	if (comp <= n)
    		return i;
    	else{
    		i = i - 2;
    		return i;
    	}
    }
}     
/*主函数*/
int main(){
    int p, q, e, d, n, fai_n, k, i,is_first=1;
    char ch,*arr,wei='a';
    printf("请输入p、q、e值，用空格间隔开\n");
    //scanf_s("%d%d%d", &p, &q, &e);  //从键盘获取p、q、e值
	scanf("%d%d%d", &p, &q, &e);  //从键盘获取p、q、e值
    n = p*q;  
    fai_n = (p-1)*(q-1);   //Φ(n)
    for (k = 0; (k*n + 1) % e != 0; k++);
    if ((k*n + 1) % e == 0)
    	d = (k*n + 1) / e;  //d * e ≡ 1 (mod Φ(n))
    k = long_n(n);
    k = k / 2;  //分组的长度
    ch = getchar(); //缓冲回车符
    arr = (char *)malloc(sizeof(char)*k);  //申请动态数组
    printf("请输入明文\n");
    while (1){
    	i=shuru(arr,k,&wei,is_first);  //调用输入字符的函数，接收到回车符返回0，否则为1
    	is_first = 0;  //第一分组录入结束设为0
    	jiami(arr,k,e,n);  //调用加密函数
    	if (i == 0)  //接收到返回值为0跳出循环
    		break;
    }
    printf("\n");
    return 0;
}

