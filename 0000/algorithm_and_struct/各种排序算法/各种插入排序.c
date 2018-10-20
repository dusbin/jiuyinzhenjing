//https://blog.csdn.net/touch_2011/article/details/6783965
//直接插入排序
/*
容易看出，要插入的记录个数为n-1，其中关键字的比较次数和记录移动次数是依赖于给出的待排序序列是否基本有序。在最佳情况下（待排序序列有序），比较次数和移动次数时间为o（1），所以时间复杂度为o（n）.在最坏情况下（待排序序列逆序）和平均时间均为o（n^2）.从上述分析中可以看出，直接插入排序适合记录数比较少、给定序列基本有序的情况。熟悉了排序过程我们发现，直接插入排序是一种稳定的原地排序算法。
*/
//把一个记录插入到有序序列中，至于要插入到有序序列中的哪个位置呢？采用的是顺序查找确定插入的位置
void  straightInsertSort(int *a,int n)
{
	int i,j;
	int temp;
	//逐个记录插入有序序列
	for(i=2;i<=n;i++){
		temp=a[i];
		//把a[i]插入有序序列
		for(j=i-1;j>=1;j--){
			if(temp<a[j]){
				a[j+1]=a[j];
			}else
				break;
		}
		a[j+1]=temp;
	}
}
//对于有序序列，折半查找的效率要高，所以在寻找插入位置时可以用折半查找。折半查找主要分为三步：1、查找要插入的位置  2、移位  3、把记录插入相应的位置
//折半查找
int binarySearch(int *a,int low,int high,int key)
{
	int mid=(low+high)/2;
	if(low>high)
		return low;
	if(a[mid]==key)
		return mid;
	else if(key<a[mid])
		return binarySearch(a,low,mid-1,key);
	else 
		return binarySearch(a,mid+1,high,key);
}
//折半插入排序是对直接插入排序的一种改进，这种改进只考虑了关键字比较次数，并没有减少移位次数，所以平均时间和最坏情况下（待排序序列逆序）时间复杂度o（n^2）,如果记录数量很大的话，这两种情况下是优于直接插入排序。再来看一下最佳情况（待排序序列有序），此时关键字比较次数并不为o（1），时间复杂度为o（n*log2n）。（其中折半查找时间复杂度o（log2n），这个在以后写查找的时候再分析，这里不做详细讲解。）。所以在记录数较小、待排序序列基本有序情况下直接插入排序优于折半插入排序。此外，折半插入排序是不稳定的原地排序，实现起来也较复杂。
//折半插入排序
void binaryInsertSort(int *a,int n)
{
	int i,j,site,temp;
	for(i=2;i<=n;i++){
		//1.折半查找要插入的位置
        site=binarySearch(a,1,i,a[i]);
		temp=a[i];
		//2.移位
		for(j=i;j>site;j--)
			a[j]=a[j-1];
		//3.插入a[i]
		a[site]=temp;
	}
}
//表插入排序
//折半插入排序相对于直接插入排序来说减少了比较次数。那么我们可不可以减少移动次数呢，答案是可以的。于是就有了表插入排序，用一个静态链表来存储待排序序列，其他操作和直接插入排序很像。主要步骤：1、初始化链表  2、取出要插入的记录 3、遍历链表寻找插入位置  4、记录插入链表中。
//表插入排序也是对直接插入排序的一种改进，这种改进只减少了移动次数，并没有减少关键字比较次数，所以平均时间和最坏情况下（待排序序列逆序）时间复杂度o（n^2）,如果记录数量很大的话，这两种情况下是优于直接插入排序。再来看一下最佳情况（待排序序列有序），关键字比较次数并为o（1），时间复杂度为o（n）。此时和直接插入排序时间复杂度一样。此外，表插入排序改变了记录的存储结构，无法顺序访问，是一种稳定的排序算法，实现起来也较复杂。
//静态链表
typedef struct
{
	int key;//关键字
	int next;//指向下一个关键字的下标
}Node,*PNode;
 
//表插入排序
void tableInsertSort(PNode list,int n)
{
	int p,head;
	int i;
	//初始化
	list[0].next=1;
	list[1].next=0;
   
    //逐个插入
	for(i=2;i<=n;i++){
		head=0;
		p=list[0].next;
		//遍历链表，寻找插入位置
		while(p!=0 && list[p].key<=list[i].key){
			head=p;
			p=list[p].next;
		}
		if(p==0){//插入的值是最大值
     		list[i].next=p;
	     	list[head].next=i;
		}else{
			list[i].next=p;
			list[head].next=i;
		}
 
	}
}
//希尔排序
//一趟增量为dk的希尔插入排序
//上述两种排序都是只考虑减少关键字比较次数或者只考虑减少关键字移动次数。有没有别的改进办法呢？我们注意到，直接插入排序适合于记录数较少、基本有序的情况。于是我们可以先将整个待排序序列分割成若干子序列分别进行直接插入排序，整个序列基本有序时，再对序列进行一次直接插入排序。这就是希尔排序。
//当给定序列记录量较大时，希尔排序性能优于直接插入排序。再希尔排序的过程中，关键字是跳跃式移动的，这样就减少了移动次数。希尔排序性能的分析是一个复杂的问题，时间与所取的增量有关。增量选取的不好可能会大大降低排序效率。
void  shellInsert(int *a,int n,int dk)
{
	int i,j,temp;
	for(i=dk+1;i<=n;i+=dk){
		temp=a[i];
		for(j=i-dk;j>=0;j-=dk)
			if(a[j]>temp)
				a[j+dk]=a[j];
			else
				break;
		a[j+dk]=temp;
	}
}
//希尔排序
void shellSort(int *a,int n)
{
	int i;
	int dk[]={5,4,3,2,1};
	for(i=0;i<5;i++)
		shellInsert(a,6,dk[i]);
}

