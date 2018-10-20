//参考资料https://blog.csdn.net/touch_2011/article/details/6767673
//选择排序:思想第i趟在剩余的元素中找最小/大的数，和i位进行替换。
//最优 0次，最差O(n**2) 复杂度 平均 最坏 最佳O(n**2)
for(int i = 0;i < n;i++){
	index = i;
	fot(int j = i+1;j < n;j++)
		if(a[j]<a[index])//判断是否需要更新索引，升序排列
			index = l;
	if(index!=i){//需要交换
		a[i] = a[i]+a[index];
		a[index] = a[i] - a[index];
		a[i] = a[i] - a[index];
	}
	
}
//递归实现
void simpleSelectionSort2(int *a,int n)
{
    int index,i;
	if(n==1)
		return;
	//1.选择待排序序列a中的最小记录,其下标为index
    for(index=i=1;i<=n;i++){
		if(a[i]<a[index])
	        index=i;
	}
	//2.最小记录与待排序序列首元素进行交换
	if(index!=1){
		a[1]=a[1]+a[index];
		a[index]=a[1]-a[index];
		a[1]=a[1]-a[index];
	}
	//3.待排序序列元素个数减少，递归对剩下的无序序列排序
	simpleSelectionSort2(a+1,n-1);
}

