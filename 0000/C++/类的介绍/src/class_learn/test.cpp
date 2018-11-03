#include <iostream>
using namespace std;
class Animal{
public:
	Animal(){
		this->name = "undefine";
	}
	Animal(string name){//方法进行重载
		this->name = name;
	}
	void Say(){//必须写返回值类型
		cout << "i am " <<getname()<<","<<Saything()<<endl;
	}
	int getNum(){
		return this->shoutNum;
	}
	void SetNum(int value){//不实现set。表明数据只读
		if(value <= 10)
			this->shoutNum = value;
		else
			this->shoutNum = 10;
	}
protected://可以被继承
	string getname(){
		return this->name;
	}
	string name;
	int shoutNum = 3;
private:
	virtual string Saything(){
		return "";
	}
};
class Cat:public Animal{
public:
	Cat(){
		this->name = "undefine";
	}
	Cat(string name){//方法进行重载
		this->name = name;
	}
private:
	string Saything(){
		string result = "";
		for(int i= 0;i<this->shoutNum;i++){
			result += "miao";
			if(i<this->shoutNum-1){
				result += " ";
			}
		}
		return result;
	}
};
class Dog:public Animal{
public:
	Dog(){
		this->name = "undefine";
	}
	Dog(string name){//方法进行重载
		this->name = name;
	}
private:
	string Saything(){
		string result = "";
		for(int i= 0;i<this->shoutNum;i++){
			result += "wang";
			if(i<this->shoutNum-1){
				result += " ";
			}
		}
		return result;
	}
};
void test_01(){
	Cat *cat = new Cat();
	cat->Say();
	Cat *cat2 = new Cat("Tian");
	cat2->SetNum(5);
	cat2->Say();
	cat2->SetNum(11);
	cat2->Say();
	Dog *dog = new Dog("du");
	dog->Say();
}
int main(){
	test_01();
	return 0;
}


