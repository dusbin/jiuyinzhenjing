/** 
	vector 常被称为“容器”，习惯将之理解为动态数组。它表示对象的集合，其中所有对象都相同，
	每个对象都对应一个索引，索引用来访问对象。作为动态数组，vector 有一个指针指向一片连续
	的内存空间。但是，这个内存空间肯定不是无限大的，当内存装不下数据时，系统会自动申请一片
	更大的空间，把原来的数据拷贝过去，释放原来的内存空间。
	--------------------- 
	作者：denghe1122 
	来源：CSDN 
	原文：https://blog.csdn.net/denghecsdn/article/details/78975954 
	版权声明：本文为博主原创文章，转载请附上博文链接！
*/
#include <iostream>
#include <vector>
#include <list>
using namespace std;
void test_01(){
	vector<int> v;
	for(int i = 0; i < 32;i++){
		v.push_back(i);
		cout<<"size: "<<v.size()<<" capacity: "<<v.capacity()<<endl;//size是实际占用大小，capacity是vector的容量大小
		//linux下是两倍增加的
		//windows下是1.5倍
	}
}
int main(){
	test_01();
	return 0;
}


