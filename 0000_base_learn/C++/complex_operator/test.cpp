#include <iostream>
using namespace std;
class Complex{
public:
	Complex(Complex &a);
	Complex(double r = 0,double i = 0);
	void display();
	void operator = (const Complex& a); //赋值操作
	Complex operator +(const Complex& a);//加法操作
	Complex operator -(const Complex& a);//减法操作 //临时变量 需要使用const
	Complex operator +(double r);//加法操作
	Complex operator -(double r);//减法操作
private:
	double real;
	double img;
};
Complex::Complex(Complex &a){
	real = a.real;
	img = a.img;
}
Complex::Complex(double r,double i){
	real = r;
	img = i;
}
void Complex::display(){
	cout << real << ( img >= 0?"+":"") << img << "i"<<endl;
}
void Complex::operator = (const Complex& a){
	real = a.real;
	img = a.img;
}
Complex Complex::operator +(const Complex& a){
	Complex temp(a.real + real,a.img + img);
	return temp;
}
Complex Complex::operator -(const Complex& a){
	Complex temp(real - a.real,img - a.img);
	return temp;
}
Complex Complex::operator +(const double r){
	Complex temp(real + r,img);
	return temp;
}
Complex Complex::operator -(const double r){
	Complex temp(real - r,img);
	return temp;
}
void test_01(){
	Complex a(3,2),b(5,4),c(1,1),d(4,2),temp;
	a.display();
	cout << "+";
	b.display();
	temp=a+b;
	temp.display();
	cout <<"-----------" <<endl;
	a.display();
	cout << "+1"<<endl;
	temp=a+1;
	temp.display();
	cout <<"-----------" <<endl;
	a.display();
	cout << "-";
	b.display();
	temp=a-b;
	temp.display();
	cout <<"-----------" <<endl;
	cout <<"a + b -(c+d)" <<endl;
	temp=a+b-(c+d);
	temp.display();
	cout <<"-----------" <<endl;
}
int main(){
	test_01();
	return 0;
}


