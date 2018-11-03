#include <iostream>
using namespace std;
class Operation{
public:
	Operation(){
	}
	void setnumberA(double value){
		this->_numberA = value;
	}
	void setnumberB(double value){
		this->_numberB = value;
	}
	virtual double GetResult(){
		double result = 0;
		return result;
	}
protected:
	double _numberA = 0;
	double _numberB = 0;
};
class Add:public Operation{
public: 
	double GetResult(){
		double result = 0;
		result = this->_numberA + this->_numberB;
		return result;
	}
};
class Sub:public Operation{
public: 
	double GetResult(){
		double result = 0;
		result = this->_numberA - this->_numberB;
		return result;
	}
};
class Mul:public Operation{
public: 
	double GetResult(){
		double result = 0;
		result = this->_numberA * this->_numberB;
		return result;
	}
};
class Div:public Operation{
public: 
	double GetResult(){
		double result = 0;
		if(this->_numberB == 0)
			return result;
		result = this->_numberA / this->_numberB;
		return result;
	}
};
void test_01(){
	Operation *oper = new Add();
	oper->setnumberA(1);
	oper->setnumberB(2);
	cout<<"+ "<<oper->GetResult()<<endl;
	delete oper;
	oper = new Sub();
	oper->setnumberA(1);
	oper->setnumberB(2);
	cout<<"- "<<oper->GetResult()<<endl;
	delete oper;
	oper = new Mul();
	oper->setnumberA(3);
	oper->setnumberB(2);
	cout<<"* "<<oper->GetResult()<<endl;
	delete oper;
	oper = new Div();
	oper->setnumberA(1);
	oper->setnumberB(2);
	cout<<"/ "<<oper->GetResult()<<endl;
	delete oper;
	oper = new Div();
	oper->setnumberA(1);
	oper->setnumberB(0);
	cout<<"/ "<<oper->GetResult()<<endl;
	delete oper;
}
int main(){
	test_01();
	return 0;
}


