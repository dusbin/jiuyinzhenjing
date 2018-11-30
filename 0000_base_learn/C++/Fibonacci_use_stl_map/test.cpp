#include <iostream>
#include <map>
using namespace std;
typedef map<int,int> M;
typedef M::value_type v_t;
M m1;
int getfrommap(int n){
	M::iterator iter = m1.begin();
	int ret = -1;
	iter = m1.find(n);
	if(iter != m1.end())
		ret = (*iter).second;
	//printf("getfrommap n = %d ret = %d\n",n,ret);
	return ret;	
}
int fibonacci(int n);
int fibonacci(int n){
	if(n == 0 || n == 1){
		return getfrommap(n);
	}
	//if(n < 0)
	//	return 0;
	int ret = -1;
	ret = getfrommap(n);
	if(ret >= 0){
		return ret;
	}
	ret = fibonacci(n-2)+fibonacci(n-1);
	if(ret > 0&&-1 == getfrommap(n)){
		m1.insert(v_t(n,ret));
		//printf("n = %d ret = %d\n",n,ret);
	}
	return ret;
}
int test_01(){
	m1.insert(v_t(0,0));
	m1.insert(v_t(1,1));
	printf("fibonacci(%d) = %d\n",0,fibonacci(0));
	printf("fibonacci(%d) = %d\n",1,fibonacci(1));
	printf("fibonacci(%d) = %d\n",2,fibonacci(2));
	printf("fibonacci(%d) = %d\n",3,fibonacci(3));
	printf("fibonacci(%d) = %d\n",4,fibonacci(4));
	printf("fibonacci(%d) = %d\n",5,fibonacci(5));
	printf("fibonacci(%d) = %d\n",6,fibonacci(6));
	printf("fibonacci(%d) = %d\n",7,fibonacci(7));
	printf("fibonacci(%d) = %d\n",8,fibonacci(8));
	printf("fibonacci(%d) = %d\n",9,fibonacci(9));
	printf("fibonacci(%d) = %d\n",10,fibonacci(10));
}
int main(){
	test_01();
	return 0;
}


