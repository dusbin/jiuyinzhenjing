#include <stdio.h>
#include <stdlib.h>
 
//虚函数表结构
struct base_vtbl
{
	void(*dance)(void *);
	void(*jump)(void *);
};
 
//基类
struct base
{
    /*virtual table*/
	struct base_vtbl *vptr;
};
 
void base_dance(void *this)
{
	printf("base dance\n");
}
 
void base_jump(void *this)
{
	printf("base jump\n");
}
 
/* global vtable for base */
struct base_vtbl base_table =
{
		base_dance,
		base_jump
};
 
//基类的构造函数
struct base * new_base()
{
    struct base *temp = (struct base *)malloc(sizeof(struct base));
	temp->vptr = &base_table;
	return temp;
}
 
 
//派生类
struct derived1
{
	struct base super;
	/*derived members */
	int high;
};
 
void derived1_dance(void * this)
{
	/*implementation of derived1's dance function */
	printf("derived1 dance\n");
}
 
void derived1_jump(void * this)
{
	/*implementation of derived1's jump function */
	struct derived1* temp = (struct derived1 *)this;
	printf("derived1 jump:%d\n", temp->high);
}
 
/*global vtable for derived1 */
struct base_vtbl derived1_table =
{
	(void(*)(void *))&derived1_dance,
	(void(*)(void *))&derived1_jump
};
 
//派生类的构造函数
struct derived1 * new_derived1(int h)
{
    struct derived1 * temp= (struct derived1 *)malloc(sizeof(struct derived1));
	temp->super.vptr = &derived1_table;
	temp->high = h;
	return temp;
}
 
 
 
int main(void)
{
 
    struct base * bas = new_base();
    //这里调用的是基类的成员函数
    bas->vptr->dance((void *)bas);
    bas->vptr->jump((void *)bas);
 
 
    struct derived1 * child = new_derived1(100);
    //基类指针指向派生类
    bas  = (struct base *)child;
 
    //这里调用的其实是派生类的成员函数
    bas->vptr->dance((void *)bas);
    bas->vptr->jump((void *)bas);
	return 0;
}
