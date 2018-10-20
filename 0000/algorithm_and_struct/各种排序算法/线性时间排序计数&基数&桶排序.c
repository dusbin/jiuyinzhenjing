//计数排序便是很基础的一种线性时间排序，它是基数排序的基础。基本思想是：对每一个元素x，确定小于x的元素个数，就可以把x直接放到它在有序序列中的位置上。过程描述：假设待排序序列a中值的范围[0,k],其中k表示待排序序列中的最大值。首先用一个辅助数组count记录各个值在a中出现的次数，比如count[i]表示i在a中的个数。然后依次改变count中元素值，使count[i]表示a中不大于i的元素个数。然后从后往前扫描a数组，a中的元素根据count中的信息直接放到辅助数组b中。最后把有序序列b复制到a。
#include<stdio.h>
#include<stdlib.h>
 
//计数排序,n为数组a的记录个数，k为记录中最大值
void countingSort(int *a,int n,int k)
{
	int i;
	int *count=(int *)malloc(sizeof(int)*(k+1));
	int *b=(int *)malloc(sizeof(int)*(n+1));
	//初始化计数数组count
	for(i=0;i<=k;i++)
		*(count+i)=0;
	//计算等于a[i]的记录个数
	for(i=1;i<=n;i++)
		(*(count+a[i]))++;
	//计算小于等于a[i]的记录个数
	for(i=1;i<=k;i++)
        *(count+i) += *(count+i-1);
	//扫描a数组，把各个元素放在有序序列中相应的位置上
	for(i=n;i>=1;i--){
		*(b + *(count + a[i]))=a[i];
        (*(count+a[i]))--; 
	}
	for(i=1;i<=n;i++)
		a[i]=*(b+i);
	free(count);
	free(b);
}
 
void main()
{
	int i;
	int a[7]={0,3,5,8,9,1,2};//不考虑a[0]
	countingSort(a,6,9);
	for(i=1;i<=6;i++)
		printf("%-4d",a[i]);
	printf("\n");
}
/*
从代码来看，计数排序有5个for循环，其中三个时间是n，两个时间是k。所以总时间T(3n+2k),时间复杂度o（n+k），不管是在最坏还是最佳情况下，此时间复杂度不变.此外，计数排序是稳定的，辅助空间n+k,这个空间是比较大的，计数排序对待排序序列有约束条件(如前面我们假设待排序序列a中值的范围[0,k],其中k表示待排序序列中的最大值)，元素值需是非负数，k太大的话会大大降低效率。这里要注意的是 “扫描a数组把各个元素放在有序序列相应的位置上” 这步为什么要从后往前扫描a数组呢？大家想一想计数排序的过程就知道，因为从前扫描导致计数排序不稳定，前面说了，计数排序是基数排序的基础，所以它的稳定性直接影响到基数排序的稳定。
--------------------- 
作者：一个程序写手 
来源：CSDN 
原文：https://blog.csdn.net/touch_2011/article/details/6787127 
版权声明：本文为博主原创文章，转载请附上博文链接！
*/
//在计数排序中，当k很大时，时间和空间的开销都会增大（可以想一下对序列{8888,1234,9999}用计数排序，此时不但浪费很多空间，而且时间方面还不如比较排序）。于是可以把待排序记录分解成个位(第一位)、十位(第二位)....然后分别以第一位、第二位...对整个序列进行计数排序。这样的话分解出来的每一位不超过9，即用计数排序序列中最大值是9.
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
 
 
//计数排序,n为数组a的记录个数，k为记录中最大值，按第d位排序
void countingSort(int *a,int n,int k,int d)
{
	int i;
	int *count=(int *)malloc(sizeof(int)*(k+1));
	int *b=(int *)malloc(sizeof(int)*(n+1));
	//初始化计数数组count
	for(i=0;i<=k;i++)
		*(count+i)=0;
	//计算等于a[i]在d位(a[i]/(int)pow(10,d-1)%10)的记录个数
	for(i=1;i<=n;i++)
        (*(count+a[i]/(int)pow(10,d-1)%10))++;
 
	//计算小于等于a[i]在d位(a[i]/(int)pow(10,d-1)%10)的记录个数
	for(i=1;i<=k;i++)
        *(count+i) += *(count+i-1);
	//扫描a数组，把各个元素放在有序序列中相应的位置上
	for(i=n;i>=1;i--){
		*(b + *(count + a[i]/(int)pow(10,d-1)%10))=a[i];
        (*(count+a[i]/(int)pow(10,d-1)%10))--; 
	}
	for(i=1;i<=n;i++)
		a[i]=*(b+i);
	free(count);
	free(b);
}
 
 
//基数排序,n为数组a的记录个数，每一个记录中有d位数字
void radixSort(int *a,int n,int d)
{
	int i;
	for(i=1;i<=d;i++){
	    countingSort(a,6,9,i);
	}
}
 
void main()
{
	int i;
	int a[7]={0,114,118,152,114,111,132};//不考虑a[0]
	radixSort(a,6,3);
	for(i=1;i<=6;i++)
		printf("%-4d",a[i]);
	printf("\n");
}
/*基数排序时间T(n)=d*(2k+3n),其中d是记录值的位数，(2k+3n)是每一趟计数排序时间，上文分析过了，k不超过9，d的值一般也很小，k、d都可以看成是一个很小的常数，所以时间复杂度o（n）。最坏最佳情况并不改变时间复杂度。基数排序是稳定的。辅助空间同计数排序k+n*/
//同计数排序一样，桶排序也对待排序序列作了假设，桶排序假设序列由一个随机过程产生，该过程将元素均匀而独立地分布在区间[0,1)上。基本思想是：把区间[0,1)划分成n个相同大小的子区间，称为桶。将n个记录分布到各个桶中去。如果有多于一个记录分到同一个桶中，需要进行桶内排序。最后依次把各个桶中的记录列出来记得到有序序列。
#include<stdio.h>
#include<stdlib.h>
 
//桶排序
void bucketSort(double* a,int n)
{
	//链表结点描述
	typedef struct Node{
		double key;
        struct Node * next; 
	}Node;
	//辅助数组元素描述
	typedef struct{
         Node * next;
	}Head;
	int i,j;
    Head head[10]={NULL};
	Node * p;
	Node * q;
	Node * node;
	for(i=1;i<=n;i++){
		node=(Node*)malloc(sizeof(Node));
		node->key=a[i];
		node->next=NULL;
		p = q =head[(int)(a[i]*10)].next;
		if(p == NULL){
			head[(int)(a[i]*10)].next=node;
			continue;
		}
		while(p){
            if(node->key < p->key)
				break;
			q=p;
			p=p->next;
		}
		if(p == NULL){
			q->next=node;
		}else{
			node->next=p;
			q->next=node;
		}
	}
	j=1;
	for(i=0;i<10;i++){
    	p=head[i].next;
		while(p){
			a[j++]=p->key;
			p=p->next;
		}
	}
}
 
void main()
{
	int i;
	double a[13]={0,0.13,0.25,0.18,0.29,0.81,0.52,0.52,0.83,0.52,0.69,0.13,0.16};//不考虑a[0]
	bucketSort(a,12);
	for(i=1;i<=12;i++)
		printf("%-6.2f",a[i]);
	printf("\n");
}
//当记录在桶中分布均匀时，即每个桶只有一个元素，此时时间复杂度o（n）。因此桶排序适合对很少重复的记录排序。辅助空间2n。桶排序是稳定的排序，实现比较复杂
