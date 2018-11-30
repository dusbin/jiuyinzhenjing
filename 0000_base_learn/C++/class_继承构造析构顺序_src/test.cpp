#include <iostream>
using namespace std;
//构造（new的时候），先父类，再子类
//析构（delete的时候），先子类，再父类
class First{
public:
	First(){
		cout<<"First"<<endl;
	}
	~First(){
		cout<<"~First"<<endl;
	}
};
class Second:public First{
public:
	Second(){
		cout<<"Second"<<endl;
	}
	~Second(){
		cout<<"~Second"<<endl;
	}
};
class Three:public Second{
public:
	Three(){
		cout<<"Three"<<endl;
	}
	~Three(){
		cout<<"~Three"<<endl;
	}
};
void test_01(){
	Three *tree = new Three();
	delete tree;
}
int main(){
	test_01();
	return 0;
}


