#include<stdio.h>
#include<stdlib.h>
#define INFINITE 1000
 
//对两个序列进行合并,数组从mid分开
//对a[start...mid]和a[start+1...end]进行合并
void merge(int *a,int start,int mid,int end)
{
	int i,j,k;
	//申请辅助数组
	int *array1=(int *)malloc(sizeof(int)*(mid-start+2));
	int *array2=(int *)malloc(sizeof(int)*(end-mid+1));
 
	//把a从mid分开分别赋值给数组
    for(i=0;i<mid-start+1;i++)
		*(array1+i)=a[start+i];
	*(array1+i)=INFINITE;//作为哨兵
    for(i=0;i<end-mid;i++)
		*(array2+i)=a[i+mid+1];
    *(array2+i)=INFINITE;
	//有序的归并到数组a中
    i=j=0;
	for(k=start;k<=end;k++){
		if(*(array1+i) > *(array2+j)){
			a[k]=*(array2+j);
			j++;
		}
		else{
			a[k]=*(array1+i);
			i++;
		}
	}
	free(array1);
	free(array2);
}
 
//归并排序
void mergeSort(int *a,int start,int end)
{
	int mid=(start+end)/2;
	if(start<end){
		//分解
		mergeSort(a,start,mid);
		mergeSort(a,mid+1,end);
		//合并
		merge(a,start,mid,end);
	}
}
 
void main()
{
	int i;
	int a[7]={0,3,5,8,9,1,2};//不考虑a[0]
	mergeSort(a,1,6);
	for(i=1;i<=6;i++)
		printf("%-4d",a[i]);
	printf("\n");
}
//归并排序又是另一类排序算法，它是一种基于“分治”策略的一种算法。归并排序算法是典型的分治算法，对于规模较大的问题，可以分解成若干容易求解的简单的问题，最后把解合并构成初始问题的解。
/*
合并排序是比较复杂的排序，特别是对于不了解分治法基本思想的同学来说可能难以理解。总时间=分解时间+解决问题时间+合并时间。分解时间就是把一个待排序序列分解成两序列，时间为一常数，时间复杂度o(1).解决问题时间是两个递归式，把一个规模为n的问题分成两个规模分别为n/2的子问题，时间为2T(n/2).合并时间复杂度为o（n）。总时间T(n)=2T(n/2)+o(n).这个递归式可以用递归树来解，其解是o(nlogn).此外在最坏、最佳、平均情况下归并排序时间复杂度均为o(nlogn).从合并过程中可以看出合并排序稳定。 

用递归树的方法解递归式T(n)=2T(n/2)+o(n):假设解决最后的子问题用时为常数c，则对于n个待排序记录来说整个问题的规模为cn。
第一层时间代价为cn，第二层时间代价为cn/2+cn/2=cn.....每一层代价都是cn，总共有logn+1层。所以总的时间代价为cn*(logn+1).时间复杂度是o(nlogn).
--------------------- 
作者：一个程序写手 
来源：CSDN 
原文：https://blog.csdn.net/touch_2011/article/details/6785881 
版权声明：本文为博主原创文章，转载请附上博文链接！
*/

