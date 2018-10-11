#include <iostream>
using namespace std;
class A
{
public:
    int get() const{return i;}
    void set(int x){this->i=x;cout<<"this指针保存的内存地址为:"<<this<<endl;}
private:
    int i;
};
 
int main()
{
    A a;
    a.set(9);
    cout<<"对象a所在的内存地址为:"<<&a<<endl;
    cout<<"对象a所保存的值为:"<<a.get()<<endl;
    cout<<endl;
    A b;
    b.set(999);
    cout<<"对象b所在的内存地址为:"<<&b<<endl;
    cout<<"对象b所保存的值为:"<<b.get()<<endl;
    return 0;
}
/*
this指针
	class理解为一种类型，像int char一样，是用户自己定义的类型，这个类型可以声明变量像int x 一样 myclass my。
	my里的this 指向my的指针。
经典解释：
	当你进入一个房子后，
	你可以看见桌子、椅子、地板等，
	但是房子你是看不到全貌了。
	对于一个类的实例来说，
	你可以看到它的成员函数、成员变量，
	但是实例本身呢？
	this是一个指针，它时时刻刻指向你这个实例本身
一个学生可以有多本书一样，而这些书都是属于这个同学的；同理，如果有很多个同学在一起，那么为了确定他们的书不要拿混淆了，最好的办法我想应该就是每个同学都在自己的书上写上名字，这样肯定就不会拿错了。
      同理，一个对象的多个成员就可看作是这个对象所拥有的书；而在很多个对象中间，我们为了证明某个成员是自己的成员，而不是其他对象的成员，我们同样需要给这些成员取上名字。在C++中，我们利用this指针帮助对象做到这一点，this指针记录每个对象的内存地址，然后通过运算符->访问该对象的成员。
 */