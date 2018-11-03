#include <iostream>
using namespace std;
class Cat{
public:
	Cat(){
		this->name = "undefine";
	}
	Cat(string name){
		this->name = name;
	}
	void Say(){//必须写返回值类型
		cout << "i am " <<getname()<<", miao!"<<endl;
	}
private:
	string getname(){
		return this->name;
	}
	string name;
};
void test_01(){
	Cat *cat = new Cat();
	cat->Say();
	Cat *cat2 = new Cat("Tian");
	cat2->Say();
}
int main(){
	test_01();
	return 0;
}


