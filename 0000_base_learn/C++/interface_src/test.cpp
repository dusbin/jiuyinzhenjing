/**
 https://blog.csdn.net/netyeaxi/article/details/80724557
 https://blog.csdn.net/netyeaxi/article/details/80887646
 */
#include <iostream>
#include <cmath>
using namespace std;
/**
 接口类具有以下特征
	最好不要有成员变量，单可以有静态常量（static const或enum）
	要有纯虚接口方法
	要有虚析构函数，并提供默认实现
	不要声明构造函数
 */
class Testable{
public:
	/**
	 如果成员变量，尤其是可变的成员变量，定义在接口中，等于是吧实现细节暴露出来了，不符
	 合接口定义的要求，所有一般不在接口中定义可变的成员变量。常量可以在接口中定义，有时接口需要返回状态
	 而这些状态可以定义成常量放在接口中。
	 */
	static const int START = 1;
	static const int STOP = 2;
	/**
	 不能让接口类自身能够实例化，并且需要子类必须实现接口暴露的方法，所以接口方法都要声明成纯虚函数。
	 声明成纯虚函数意味着接口类自身不需要提供方法的定义，方法的定义需要由接口类的子类提供，并且接口类自身也因此变成了
	 抽象类而不能被实例化
	 */
	virtual void test() = 0;
	/**
	 a.在使用接口类的指针访问接口类的子类的实例时，当对接口类的指针做delete时，如果接口类的析构函数不是虚拟析构函数，
	   将只会调用接口类的析构函数，接口类的子类的析构函数将不会被调用。内存泄漏将会产生。所以接口类的析构函数必须定义
	   成虚析构函数
	 b.如果接口类的析构函数不提供默认实现，即如果接口类的析构函数是纯虚析构函数的话，接口类的子类将被迫必须提供析构函数
	   的实现，这样对接口类的子类不友好
	 c.在C++11中也可以用 virtual ~Testable() = default;替代 virtual ~Testable(){};
	 */
	 /**
	 不要显示定义任何的构造函数，但也不要在接口中加入如下代码来禁止生产构造函数
		Testable() = delete;
		Testable(const Testable&) = delete;
	 因为C++的调用机制要求子类的构造函数调用时一定会先调用父类的构造函数，如果禁止生产构造函数，代码编译时会报错，如果程序员
	 不显示的提供构造函数，编译器也会隐式的加上构造函数的，损人这些构造函数对于接口类来说没有实际意义。
	 */
	 /**
	  只要对纯虚析构函数提供一个默认实现就可以了，这种对纯虚函数提供实现的方法看似很奇怪，但是C++是支持的
	  */
	virtual ~Testable(){}; //从C++11开始可以：virtual ~Testable() = default;
};
//圆的接口类
class Graph{
public:
	virtual ~Graph(){};
	//接口方法：面积
	virtual double area() = 0;
	//接口方法：周长
	virtual double lang() = 0;
};
//圆的具体实现类
class Circle:public Graph{
private:
	double radius;//半径
public:
	Circle(double radius);
	double area() override;
	double lang() override;
};
inline double pi(){
	return std::atan(1) * 4;
};
//构造方法
Circle::Circle(double _radius) : radius(_radius){
	
};
double Circle::area(){
	return pi() * radius * radius;
};
double Circle::lang(){
	return pi() * radius * 2;
};
//圆的具体实现类
class Square:public Graph{
private:
	double radius;//半径
public:
	Square(double radius);
	double area() override;
	double lang() override;
};
//构造方法
Square::Square(double _radius) : radius(_radius){
	
};
double Square::area(){
	return radius * radius;
};
double Square::lang(){
	return radius * 4;
};
//圆的创建工厂类
class Manger{
public:
	Graph* create(double radius,int flag);//创建circle实例
	static void destroy(Graph* graphPtr);//销毁circle实例
};
Graph* Manger::create(double radius,int flag){
	Graph *graphPtr = NULL;
	if(flag == 1){//圆型
		return graphPtr = new Circle(radius);
	}else{
		return graphPtr = new Square(radius);
	}
	//return graphPtr;
};
void Manger::destroy(Graph* circlePtr){
	delete circlePtr;
};
void test_01(){
	double a = 2;
	//Graph* circlePtr = Manger::create(a);
	Manger *mng = new Manger;
	Graph* graphPtr = mng->create(a,1);//圆
	cout<<"area = "<<graphPtr->area()<<" lang = "<<graphPtr->lang()<<endl;
	mng->destroy(graphPtr);
	graphPtr = mng->create(a,0);//正方形
	cout<<"area = "<<graphPtr->area()<<" lang = "<<graphPtr->lang()<<endl;
	mng->destroy(graphPtr);
	a = 5.5;
	graphPtr = mng->create(a,1);//圆
	cout<<"area = "<<graphPtr->area()<<" lang = "<<graphPtr->lang()<<endl;
	mng->destroy(graphPtr);
	graphPtr = mng->create(a,0);//正方形
	cout<<"area = "<<graphPtr->area()<<" lang = "<<graphPtr->lang()<<endl;
	mng->destroy(graphPtr);
	//Manger::destroy(circlePtr);
}
int main(){
	test_01();
	return 0;
}


