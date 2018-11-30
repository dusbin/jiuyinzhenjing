#include <iostream>
#include <vector>
#include <list>
using namespace std;
//加锁实现懒汉，第一次用的时候再初始化
class singleton_a{
protected:
	singleton_a(){
		pthread_mutex_init(&mutex);//线程安全
	}
private:
	static singleton_a* p;
public:
	static singleton_a* instance();
	static pthread_mutex_t mutex;//线程锁
};
singleton_a* singleton_a::instance(){
	if(p = NULL){
		pthread_mutex_lock(&mutex);//加锁
		if(p == NULL)
			p = new singleton_a();
		pthread_mutex_unlock(&mutex);//解锁
	}
	return p;
}
//静态变量的懒汉实现
class singleton_b{
protected:
	singleton_b(){
		pthread_mutex_init(&mutex);//线程安全
	}
public:
	static singleton_b* instance();
	static pthread_mutex_t mutex;//线程锁
	int a;
};
pthread_mutex_t singleton_b::mutex;
singleton_b* singleton_b::instance(){
	pthread_mutex_lock(&mutex);//加锁
	static singleton_b obj
	pthread_mutex_unlock(&mutex);//解锁
	return &obj;
}
//饿汉实现,以空间换时间，一开始就生成好
class singleton_c{
protected:
	singleton_c(){
	}
private:
	static singleton_c* p;
public:
	static singleton_c* instance();
};
pthread_mutex_t singleton_c::p = new singleton_c;
singleton_c* singleton_c::instance(){
	return p;
}
void test_01(){

}
int main(){
	test_01();
	return 0;
}


