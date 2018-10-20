//https://blog.csdn.net/touch_2011/article/details/6767673
//堆，是一颗完全二叉树且满足所有非叶子节点的值均不大于或均不小于其左右孩子节点
//分为大顶堆和小顶堆
//所有节点满足A[i]<=A[2*i]&&a[i]<=A[2*i+1] 注意2*i和2*i+1是否存在
//时间复杂度为O（log2n）建堆的时间复杂度为O（n*log2n）
//调整堆，保持大顶堆的性质,参数i指向根结点
void maxHeap(int *a,int n,int i)
{
	//left、right、largest分别指向
	//左孩子、右孩子、{a[i],a[left]}中最大的一个
	int left,right,largest;
	largest=left=2*i;
	if(left>n)
		return;
	right=2*i+1;
	if(right<=n && a[right]>a[left]){
        largest=right;
	}
	if(a[i]<a[largest]){//根结点的值不是最大时,交换a[i]，a[largest]
		a[i]=a[i]+a[largest];
		a[largest]=a[i]-a[largest];
        a[i]=a[i]-a[largest];
		//自上而下调整堆
		maxHeap(a,n,largest);
	}
}
//建堆
void creatHeap(int *a,int n)
{
	int i;
	//自下而上调整堆
	for(i=n/2;i>=1;i--)
		maxHeap(a,n,i);
}
//堆排序 堆排序时间复杂度O（n*log2n）
void heapSort(int *a,int n)
{
	int i;
	creatHeap(a,n);//建堆
	for(i=n;i>=2;i--){
		//堆顶记录和最后一个记录交换
		a[1]=a[1]+a[i];
		a[i]=a[1]-a[i];
		a[1]=a[1]-a[i];
		//堆中记录个数减少一个，筛选法调整堆
		maxHeap(a,i-1,1);
	}
}
/**
	反思堆排序 ------ 揭开选择排序的本质和基本思想

        叙述到这里，堆排序就叙述完了。不知道大家发现没有，上文中每一个树形的堆边上都有一个数组，其实待排序的整个过程中都是数组元素在不断的交换移动，树形的堆只是能形象的表示这个过程。通过观察这个数组的变化，我们发现了什么呢？

        仔细回想一下筛选法调整堆的过程我们发现，第i次调整堆，其实就是把A中的第i大元素放到首位置A[1]，然后A[1]和A[n-i+1]互换.这样A[(n-i+1)...n]就已经有序，于是又把A[1...n-i]看成一个堆再进行排序，如此重复。

        还记得3.1中提到那个简单的问题（选择出A中最小的记录）吗？调整堆不就是选择出待排序序列中的最大值吗？所以堆排序的本质和选择排序的本质是一样的。选择一个待排序序列中的最小（大）值，这就是选择排序的本质。

        叙述到此，相信大家已然知道开篇提出的两个问题的答案了吧。选择排序的基本思想是：每一趟从n-i+1个记录中选择最小（大）记录和第i（n-i+1）个记录交换。
--------------------- 
作者：一个程序写手 
来源：CSDN 
原文：https://blog.csdn.net/touch_2011/article/details/6767673 
版权声明：本文为博主原创文章，转载请附上博文链接！

 */

