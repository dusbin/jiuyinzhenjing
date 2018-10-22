#include <iostream>
using namespace std;
class A{
public:
	A(){m_A = 1;}
	~A(){}
	virtual int funA(){cout << "funA"<<endl;return 0;}
	int m_A;
};
class B{
public:
	B(){m_B = 2;}
	~B(){}
	virtual int funB(){cout << "funB" << endl;return 0;}
	int m_B;
};
class C{
public:
	C(){m_C = 3;}
	~C(){}
	virtual int funC(){cout << "funC" << endl;return 0;}
	int m_C;
};
class D:public A,public B,public C{
public:
	D(){m_D = 4;}
	~D(){}
	virtual int funD(){cout << "fun" << endl;return 0;}
	int m_D;
};
//继承了A,B,C的方法 virtual函数的用法
void test_01(){
	D* pD = new D;
	//return 0;
	pD->funD();
	pD->funA();
	pD->funB();
	pD->funC();
}
int main(){
	test_01();
	return 0;
}


