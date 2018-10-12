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
//使用unique_ptr时编译出错，与auto_ptr一样，unique_ptr也采用所有权模型，但在使用unique_ptr时，程序不会等到运行阶段崩溃，而在编译器因下述代码行出现错误
void test_4(){
	unique_ptr<string> films[5] = {
		unique_ptr<string>(new string("Fowl Balls")),
		unique_ptr<string>(new string("Duck Walks")),
		unique_ptr<string>(new string("Chicken Runs")),
		unique_ptr<string>(new string("Turkey Errors")),
		unique_ptr<string>(new string("Goose Eggs"))
	};
	unique_ptr<string>pwin;
	//有下边一行编译不过
	//pwin = films[2];// films[2] loses ownership. 将所有权从films[2]转让给pwin，此时films[2]不再引用该字符串从而变成空指针
	cout << "The nominees for best avian baseballl film are\n";
	for(int i = 0; i < 5; ++i)
		cout << *films[i] << endl;
	cout << "The winner is " << *pwin << endl;
	//cin.get();
}
//shared_ptr使用
void test_5(){
	int a = 10;
    std::shared_ptr<int> ptra = std::make_shared<int>(a);
	std::shared_ptr<int> ptra2(ptra); //copy
	std::cout << ptra.use_count() << std::endl;

	int b = 20;
	int *pb = &a;
	//std::shared_ptr<int> ptrb = pb;  //error
	std::shared_ptr<int> ptrb = std::make_shared<int>(b);
	ptra2 = ptrb; //assign
	pb = ptrb.get(); //获取原始指针

	std::cout << ptra.use_count() << std::endl;
	std::cout << ptrb.use_count() << std::endl;
}
//unique_ptr“唯一”拥有其所指对象，同一时刻只能有一个unique_ptr指向给定对象（通过禁止拷贝语义、只有移动语义来实现）。相比与原始指针unique_ptr用于其RAII的特性，使得在出现异常的情况下，动态资源能得到释放。unique_ptr指针本身的生命周期：从unique_ptr指针创建时开始，直到离开作用域。离开作用域时，若其指向对象，则将其所指对象销毁(默认使用delete操作符，用户可指定其他操作)。unique_ptr指针与其所指对象的关系：在智能指针生命周期内，可以改变智能指针所指对象，如创建智能指针时通过构造函数指定、通过reset方法重新指定、通过release方法释放所有权、通过移动语义转移所有权。
void test_6(){
	std::unique_ptr<int> uptr(new int(10));  //绑定动态对象
	//std::unique_ptr<int> uptr2 = uptr;  //不能賦值
	//std::unique_ptr<int> uptr2(uptr);  //不能拷貝
	std::unique_ptr<int> uptr2 = std::move(uptr); //轉換所有權
	uptr2.release(); //释放所有权
}
//weak_ptr是为了配合shared_ptr而引入的一种智能指针，因为它不具有普通指针的行为，没有重载operator*和->,它的最大作用在于协助shared_ptr工作，像旁观者那样观测资源的使用情况。weak_ptr可以从一个shared_ptr或者另一个weak_ptr对象构造，获得资源的观测权。但weak_ptr没有共享资源，它的构造不会引起指针引用计数的增加。使用weak_ptr的成员函数use_count()可以观测资源的引用计数，另一个成员函数expired()的功能等价于use_count()==0,但更快，表示被观测的资源(也就是shared_ptr的管理的资源)已经不复存在。weak_ptr可以使用一个非常重要的成员函数lock()从被观测的shared_ptr获得一个可用的shared_ptr对象， 从而操作资源。但当expired()==true的时候，lock()函数将返回一个存储空指针的shared_ptr。
void test_7(){
	std::shared_ptr<int> sh_ptr = std::make_shared<int>(10);
	std::cout << sh_ptr.use_count() << std::endl;

	std::weak_ptr<int> wp(sh_ptr);
	std::cout << wp.use_count() << std::endl;

	if(!wp.expired()){
		std::shared_ptr<int> sh_ptr2 = wp.lock(); //get another shared_ptr
		*sh_ptr = 100;
		std::cout << wp.use_count() << std::endl;
	}
}
//循环引用
class Child;
class Parent;

class Parent {
private:
    //Child* myChild;
	std::shared_ptr<Child> ChildPtr;
public:
	/*
    void setChild(Child* ch) {
        this->myChild = ch;
    }
	*/
	void setChild(std::shared_ptr<Child> child){
		this->ChildPtr = child;
	}

    void doSomething() {
        //if (this->myChild) {
		if(this->ChildPtr.use_count()){
        }
    }

    ~Parent() {
        //delete myChild;
    }
};

class Child {
private:
    //Parent* myParent;
	std::shared_ptr<Parent> ParentPtr;
public:
	/*
    void setPartent(Parent* p) {
        this->myParent = p;
    }
	*/
	void setPartent(std::shared_ptr<Parent> parent){
		this->ParentPtr = parent;
	}
    void doSomething() {
        //if (this->myParent) {
		if(this->ParentPtr.use_count()){
        }
    }
    ~Child() {
        //delete myParent;
    }
};
void test_8(){
	//Parent* p = new Parent;
	//Child* c = new Child;
	//p->setChild(c);
	//c->setPartent(p);
	//delete c;
	std::weak_ptr<Parent> wpp;
    std::weak_ptr<Child> wpc;
    {
        std::shared_ptr<Parent> p(new Parent);
        std::shared_ptr<Child> c(new Child);
        p->setChild(c);
        c->setPartent(p);
        wpp = p;
        wpc = c;
        std::cout << p.use_count() << std::endl; // 2
        std::cout << c.use_count() << std::endl; // 2
    }
    std::cout << wpp.use_count() << std::endl;  // 1
    std::cout << wpc.use_count() << std::endl;  // 1
}
int main()
{
	//test_1();
	//test_2();
	//test_3();
	//test_4();
	//test_5();
	//test_6();
	//test_7();
	test_8();
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
	为什么摒弃auto_ptr 一句话 避免潜在的内存崩溃问题。
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
	unique_ptr为何优于auto_ptr？
		C++11中的unique_ptr是auto_ptr的替代品，它与auto_ptr一样拥有唯一拥有权的特性，与auto_ptr不一样的是，
		unique_ptr是没有复制构造函数的，这就防止了一些“悄悄地”丢失所有权的问题发生，如果需要将所有权进行转移，可以这样操作：
			unique_ptr<int> up3 = move(up1); // 现在up3是数据唯一的unique_ptr智能指针
			// 或者
			unique_ptr<int> up4(move(up1));
		auto_ptr<string> p1(new string ("auto") ； //#1 
		auto_ptr<string> p2; //#2 
		p2 = p1; //#3
		在语句#3中，p2接管string对象的所有权后，p1的所有权将被剥夺。前面说过，这是好事，
		可防止p1和p2的析构函数试图刪同—个对象；但如果程序随后试图使用p1，这将是件坏事，因为p1
		不再指向有效的数据
		unique_ptr<string> p3 (new string ("auto"); //#4 
		unique_ptr<string> p4； //#5 
		p4 = p3; //#6
		编译器认为语句#6非法，避免了p3不再指向有效数据的问题。因此，unique_ptr比auto_ptr更安全。
		但unique_ptr还有更聪明的地方。
		有时候，会将一个智能指针赋给另一个并不会留下危险的悬挂指针。假设有如下函数定义：
		unique_ptr<string> demo(const char * s)
		{
			unique_ptr<string> temp (new string (s))； 
			return temp；
		}
		并且写了如下的代码
			unique_ptr<string> ps;
			ps = demo('Uniquely special")；
		demo()返回一个临时unique_ptr，然后ps接管了原本归返回的unique_ptr所有的对象，而返回时临时的 unique_ptr 被销毁，也就是说没有机会使用 unique_ptr 来访问无效的数据，换句话来说，这种赋值是不会出现任何问题的，即没有理由禁止这种赋值。实际上，编译器确实允许这种赋值，这正是unique_ptr更聪明的地方。

		总之，党程序试图将一个 unique_ptr 赋值给另一个时，如果源 unique_ptr 是个临时右值，编译器允许这么做；如果源 unique_ptr 将存在一段时间，编译器将禁止这么做，比如：

		unique_ptr<string> pu1(new string ("hello world"));
		unique_ptr<string> pu2;
		pu2 = pu1;                                      // #1 not allowed
		unique_ptr<string> pu3;
		pu3 = unique_ptr<string>(new string ("You"));   // #2 allowed

		其中#1留下悬挂的unique_ptr(pu1)，这可能导致危害。而#2不会留下悬挂的unique_ptr，因为它调用 unique_ptr 的构造函数，该构造函数创建的临时对象在其所有权让给 pu3 后就会被销毁。这种随情况而已的行为表明，unique_ptr 优于允许两种赋值的auto_ptr 。

		当然，您可能确实想执行类似于#1的操作，仅当以非智能的方式使用摒弃的智能指针时（如解除引用时），这种赋值才不安全。要安全的重用这种指针，可给它赋新值。C++有一个标准库函数std::move()，让你能够将一个unique_ptr赋给另一个。下面是一个使用前述demo()函数的例子，该函数返回一个unique_ptr<string>对象：
		使用move后，原来的指针仍转让所有权变成空指针，可以对其重新赋值。

		unique_ptr<string> ps1, ps2;
		ps1 = demo("hello");
		ps2 = move(ps1);
		ps1 = demo("alexia");
		cout << *ps2 << *ps1 << endl;

 
		shared_ptr

		在最开始的那段代码中，也简单的使用了一下shared_ptr。shared_ptr名如其名，它允许多个该智能指针共享地“拥有”同一堆分配对象的内存；由于它的资源是可以共用的，所以也就可以透过operator=等方法，来分享shared_ptr所使用的资源。由于shared_ptr内部实现上使用的是引用计数这种方法，所以一旦一个shared_ptr指针放弃了“所有权”，其它的shared_ptr对对象的引用并不会发生变化；只有在引用计数归零的时候，shared_ptr才会真正的释放所占有的堆内存空间的。

		shared_ptr指向数组
		在默认情况下，shared_ptr将调用delete进行内存的释放；当分配内存时使用new[]时，我们需要对应的调用delete[]来释放内存；为了能正确的使用shared_ptr指向一个数组，我们就需要定制一个删除函数，例如：
		#include <iostream>
		#include <memory>
		using namespace std;
		 
		class A
		{
		public:
			A() { cout<<"constructor"<<endl; }
			~A() { cout<<"destructor"<<endl; }
		};
		 
		int main()
		{
			shared_ptr<A> arrayObj(new A[5], [](A *p){delete[] p;});
		 
			return 0;
		}
		如何选择智能指针
			1.如果程序要使用多个指向同一个对象的指针，应选择shared_ptr。这样的情况包括：
				有一个指针数组，并使用一些辅助指针来标示特定的元素，如最大的元素和最小的元素；
				两个对象包含都指向第三个对象的指针；
				STL容器包含指针。很多STL算法都支持复制和赋值操作，这些操作可用于shared_ptr，但不能用于unique_ptr（编译器发出warning）和auto_ptr（行为不确定）。如果你的编译器没有提供shared_ptr，可使用Boost库提供的shared_ptr。
			2.如果程序不需要多个指向同一个对象的指针，则可使用unique_ptr。如果函数使用new分配内存，并返还指向该内存的指针，将其返回类型声明为unique_ptr是不错的选择。这样，所有权转让给接受返回值的unique_ptr，而该智能指针将负责调用delete。可将unique_ptr存储到STL容器在那个，只要不调用将一个unique_ptr复制或赋给另一个算法（如sort()）
		理解智能指针需要从下面三个层次：

			从较浅的层面看，智能指针是利用了一种叫做RAII（资源获取即初始化）的技术对普通的指针进行封装，这使得智能指针实质是一个对象，行为表现的却像一个指针。
			智能指针的作用是防止忘记调用delete释放内存和程序异常的进入catch块忘记释放内存。另外指针的释放时机也是非常有考究的，多次释放同一个指针会造成程序崩溃，这些都可以通过智能指针来解决。
			智能指针还有一个作用是把值语义转换成引用语义。C++和Java有一处最大的区别在于语义不同，在Java里面下列代码：
		shared_ptr的使用

			shared_ptr多个指针指向相同的对象。shared_ptr使用引用计数，每一个shared_ptr的拷贝都指向相同的内存。每使用他一次，内部的引用计数加1，每析构一次，内部的引用计数减1，减为0时，自动删除所指向的堆内存。shared_ptr内部的引用计数是线程安全的，但是对象的读取需要加锁。
			初始化。智能指针是个模板类，可以指定类型，传入指针通过构造函数初始化。也可以使用make_shared函数初始化。不能将指针直接赋值给一个智能指针，一个是类，一个是指针。例如std::shared_ptr<int> p4 = new int(1);的写法是错误的
			拷贝和赋值。拷贝使得对象的引用计数增加1，赋值使得原对象引用计数减1，当计数为0时，自动释放内存。后来指向的对象引用计数加1，指向后来的对象。
			get函数获取原始指针
			注意不要用一个原始指针初始化多个shared_ptr，否则会造成二次释放同一内存
			注意避免循环引用，shared_ptr的一个最大的陷阱是循环引用，循环，循环引用会导致堆内存无法正确释放，导致内存泄漏。循环引用在weak_ptr中介绍。

		--------------------- 
		作者：ZhikangFu 
		来源：CSDN 
		原文：https://blog.csdn.net/ZhikangFu/article/details/51866988?utm_source=copy 
		版权声明：本文为博主原创文章，转载请附上博文链接！

 */