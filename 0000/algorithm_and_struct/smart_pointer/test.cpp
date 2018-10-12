#include <iostream>
//#include <tr1/memory>
#include <memory>
#include <string>
using namespace std;
class report{
private:
	std::string str;
public:
	report(const std::string s):str(s){
		std::cout << "Object created.\n";
	}
	~report(){
		std::cout << "Object deleted.\n";
	}
	void comment() const{
		std::cout << str << "\n";
	}
};
void test_1(){
	{
		std::auto_ptr<report> ps(new report("using auto ptr"));
		ps->comment();
	}
	{
		std::shared_ptr<report> ps(new report("using shared ptr"));
		ps->comment();
	}
	{
		std::unique_ptr<report> ps(new report("using unique ptr"));
		ps->comment();
	}
}
//会崩溃，原因是films[2]不再引用该字符串从而变成空指针
void test_2(){
	auto_ptr<string> films[5] = {
		auto_ptr<string>(new string("Fowl Balls")),
		auto_ptr<string>(new string("Duck Walks")),
		auto_ptr<string>(new string("Chicken Runs")),
		auto_ptr<string>(new string("Turkey Errors")),
		auto_ptr<string>(new string("Goose Eggs"))
	};
	auto_ptr<string>pwin;
	pwin = films[2];// films[2] loses ownership. 将所有权从films[2]转让给pwin，此时films[2]不再引用该字符串从而变成空指针
	cout << "The nominees for best avian baseballl film are\n";
	for(int i = 0; i < 5; ++i)
		cout << *films[i] << endl;
	cout << "The winner is " << *pwin << endl;
	//cin.get();
}
//test_2修改为shared_ptr就不会崩溃了
//使用shared_ptr时运行正常，因为shared_ptr采用引用计数，pwin和films[2]都指向同一块内存，在释放空间时因为事先要判断引用计数值的大小因此不会出现多次删除一个对象的错误。
void test_3(){
	shared_ptr<string> films[5] = {
		shared_ptr<string>(new string("Fowl Balls")),
		shared_ptr<string>(new string("Duck Walks")),
		shared_ptr<string>(new string("Chicken Runs")),
		shared_ptr<string>(new string("Turkey Errors")),
		shared_ptr<string>(new string("Goose Eggs"))
	};
	shared_ptr<string>pwin;
	pwin = films[2];// films[2] loses ownership. 将所有权从films[2]转让给pwin，此时films[2]不再引用该字符串从而变成空指针
	cout << "The nominees for best avian baseballl film are\n";
	for(int i = 0; i < 5; ++i)
		cout << *films[i] << endl;
	cout << "The winner is " << *pwin << endl;
	//cin.get();
}
int main()
{
	//test_1();
	//test_2();
	test_3();
    return 0;
}
/*
	g++版本过低会出现错误 参考 https://stackoverflow.com/questions/8171444/c-stdshared-ptr-usage-and-information
	需要加编译选项 -std=c++0x
	经典的例子：
		智能指针主要是为了应对申请的内存未释放，实现当函数程序结束时自动释放内存
		void remodel(std::string & str)
		{
			std::string * ps = new std::string(str);
			...
			if (weird_thing()) //当出现异常是，delete不被执行，导致内存泄露
				//delete ps;//应该加上这行代码进行释放，单大多数人会忘记
				throw exception();
			str = *ps; 
			delete ps;
			return;
		}
		当函数终止（不管是正常终止，还是异常终止，本地变量都将自动从栈内存中删除，因此指针ps占据的内存将被释放，如果ps指向的内存自动
		释放）
		析构函数有这个功能。如果ps有一个析构函数，该析构函数将在ps过期时自动释放它指向的内存。但ps的问题在于，它只是一个常规指针，不
		是有析构凼数的类对象指针。如果它指向的是对象，则可以在对象过期时，让它的析构函数删除指向的内存。
		这正是 auto_ptr、unique_ptr和shared_ptr这几个智能指针背后的设计思想。将基本类型指针封装为类
		对象指针（这个类肯定是个模板，以适应不同基本类型的需求），并在析构函数里编写delete语句删除指针指向的内存空间。
	四种智能指针：auto_ptr、unique_ptr、shared_ptr和weak_ptr（暂不讨论）
	为什么摒弃auto_ptr
		auto_ptr< string> ps (new string ("I reigned lonely as a cloud.”）;
		auto_ptr<string> vocation; 
		vocaticn = ps;
		上述赋值语句将完成什么工作呢？如果ps和vocation是常规指针，则两个指针将指向同一个string对象。这是不能接受的，
		因为程序将试图删除同一个对象两次——一次是ps过期时，另一次是vocation过期时。要避免这种问题，方法有多种：
			1.定义陚值运算符，使之执行深复制。这样两个指针将指向不同的对象，其中的一个对象是另一个对象的副本，缺点是
			  浪费空间，所以智能指针都未采用此方案。
			2.建立所有权（ownership）概念。对于特定的对象，只能有一个智能指针可拥有，这样只有拥有对象的智能指针的构
			  造函数会删除该对象。然后让赋值操作转让所有权。这就是用于auto_ptr和unique_ptr 的策略，但unique_ptr的策略更严格。
			3.创建智能更高的指针，跟踪引用特定对象的智能指针数。这称为引用计数。例如，赋值时，计数将加1，而指针过期时，
			  计数将减1,。当减为0时才调用delete。这是shared_ptr采用的策略。

 */