#include <iostream>
#include "demo_BTHeader.h"
#include <stack>
using namespace std;
template <typename T>BTNodeClass<T>::BTNodeClass()
{
	r = NULL;
}
template <typename T>BTNodeClass<T>::~BTNodeClass()
{
	cout<<"wo bei jiexi";
	DestroyBTree(r);
	
}
template <typename T>void BTNodeClass<T>::CreatBTNode(char *str)
{
	BTNode<T> *st[MAXSIZE];
	BTNode<T> *p = NULL;
	int top = -1;
	int k = 0,j = 0;
	char ch = '\0';
	while(str[j] != '\0')
	{
		ch = str[j];
		switch(ch)
		{
		case '(':top++;st[top] = p; k = 1;break;// K = 1意思是当遇到左括号时节点作为左孩子节点
		case ')':top--;break;
		case ',':k = 2;break;// K = 2意思是当遇到逗号时节点作为友孩子节点
		default:
			p = new BTNode<T>();
			p->lchild = p->rchild = NULL;
			p->data = ch;
			if(r == NULL) r = p;  //如果还没有节点，则p就是根节点
			else
			{
				switch(k)
				{
				case 1:st[top]->lchild = p;break;
				case 2:st[top]->rchild = p;break;
				}
			}
			break;
		}
		j++;
	}
}
template <typename T>int BTNodeClass<T>::BTNodeHeight()
{
	return BTNodeHeight1(r);
}
template <typename T>int BTNodeClass<T>::BTNodeHeight1(BTNode<T> *t)
{
	int RChildH,LChildH;
	if(t == NULL)
	{
		return 0;
	}
	else
	{
		LChildH = BTNodeHeight1(t->lchild);
		RChildH = BTNodeHeight1(t->rchild);
		return (LChildH > RChildH)?(LChildH+1):(RChildH+1);
	}
}
template <typename T>BTNode<T> * BTNodeClass<T>::FindNode(T x)
{
	return FindNode1(r,x);
}
template <typename T>BTNode<T> * BTNodeClass<T>::FindNode1(BTNode<T> *t,T x)
{
	BTNode<T> *p = NULL;
	if(t == NULL)
	{
		return NULL;
	}
	else if(t->data == x) return t;
	else
	{
		p = FindNode1(t->lchild,x);//在左子树种查找
		if(p != NULL)
		{
			return p;
		}
		else 
			return FindNode1(t->rchild,x);//在右子树中查找结果
	}
}
template <typename T>void BTNodeClass<T>::DispBTNode()
{
	DispBTNode1(r);
}
template <typename T>void BTNodeClass<T>::DispBTNode1(BTNode<T> *t)
{
	if(t == NULL)
	{
		return ;
	}
	cout<<t->data;
	if(t->lchild != NULL || t->rchild != NULL)
	{
		cout<<"(";
		DispBTNode1(t->lchild);
		//cout<<t->data;		
		cout<<",";
		DispBTNode1(t->rchild);
		cout<<")";
	}
}
template <typename T>void BTNodeClass<T>::DestroyBTree(BTNode<T> *r)
{
	cout<<"我已经》》》》》";
	if(r != NULL)
	{
		DestroyBTree(r->lchild);
		DestroyBTree(r->rchild);
		delete r;
	}
}
template <typename T>void BTNodeClass<T>::PreOrder(BTNodeClass<T> &obj)
{
	PreOrder1(obj.r);
}
template <typename T>void BTNodeClass<T>::PreOrder1(BTNode<T> *t)
{
	if(t != NULL)
	{
		cout<<t->data;
		PreOrder1(t->lchild);
		PreOrder1(t->rchild);
	}
}
template <typename T>void BTNodeClass<T>::InOrder(BTNodeClass<T> &obj)
{
	InOrder1(obj.r);
}
template <typename T>void BTNodeClass<T>::InOrder1(BTNode<T> *t)
{
	if(t != NULL)
	{
		InOrder1(t->lchild);
		cout<<t->data;
		InOrder1(t->rchild);
	}
}
template <typename T>void BTNodeClass<T>::PostOrder(BTNodeClass<T> &obj)
{
	PostOrder1(obj.r);
}
template <typename T>void BTNodeClass<T>::PostOrder1(BTNode<T> *t)
{
	if(t != NULL)
	{
		PostOrder1(t->lchild);
		PostOrder1(t->rchild);
		cout<<t->data;
	}
}
//void main11()
//{
//	BTNodeClass<char> mych; 
//	char *mystr = "A(B(D(,G)),C(E,F))";
//	BTNode<char> *temp = NULL;
//	mych.CreatBTNode(mystr);
//	cout<<"前序遍历如下"<<endl;
//	mych.DispBTNode();
//	cout<<endl;
//	cout<<"中序遍历如下"<<endl;
//	mych.InOrder(mych);
//	cout<<endl;
//	mych.PreOrder(mych);
//	cout<<endl;
//	mych.PostOrder(mych);
//	cout<<endl;
//	temp = mych.FindNode('G');
//	cout<<temp->data<<endl;
//	cout<<mych.BTNodeHeight()<<endl;
//	system("pause");
//}