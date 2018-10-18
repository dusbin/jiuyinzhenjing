#include <iostream>
#include "demo01_BTNodeClass.cpp"
using namespace std;
template <typename T>class Post
{
	/*******************************以下是二叉树的非递归算法************************************************/
public:
	void CreateBT2(BTNodeClass<T> &obj,T *prestr,T *instr,int n);//采用先序和中序遍历确定二叉树,使用递归算法
	void CreateBT3(BTNodeClass<T> &obj,T *posstr,T *instr,int n);//采用后序和中序遍历确定二叉树,使用递归算法

	//****************************此算法全部采用堆栈的数据结构********************************************

	void NPreOrder1(BTNodeClass<T> &obj);//先序遍历的第一种算法
	void NPreOrder2(BTNodeClass<T> &obj);//先序遍历的第二种算法
	void NInOrder1(BTNodeClass<T> &obj);//中序遍历的算法
	void NPostOrder1(BTNodeClass<T> &obj);//后续遍历的第一种算法
	bool Ancestor31(BTNodeClass<T> &obj,T x);//采用后续遍历进行对某一节点的祖先节点的输出
	//***************************************************************************************

	//*************************采用队列的思想按照层次遍历进行输出*****************************
	void LevelOrder1(BTNodeClass<T> &obj);//层次遍历算法
	bool Ancestor32(BTNodeClass<T> &obj,T x);//输出x节点的所有祖先节点，用层次遍历的思想
	
private:
	BTNode<T> * CreateBT21(T *prestr,int ipre,T *instr,int iin,int n);//采用先序和中序遍历确定二叉树，使用递归算法
	BTNode<T> * CreateBT31(T *posstr,int ipos,T *instr,int iin,int n);//采用后序和中序遍历确定二叉树，使用递归算法

	//****************************此算法全部采用堆栈的数据结构********************************************
	void NPreOrder11(BTNode<T> *t);
	void NPreOrder22(BTNode<T> *t);
	void NInOrder11(BTNode<T> *t);
	void NPostOrder11(BTNode<T> *t);
	bool Ancestor311(BTNode<T> *t,T x);
	/********************************非递归算法的成员函数***************************************************/

	//*************************采用队列的思想按照层次遍历进行输出*****************************
	void LevelOrder11(BTNode<T> *t);
	bool Ancestor322(BTNode<T> *t,T x);

};
template <typename T>struct QUType
{
	BTNode<T> *node;
	int parent;
};
//利用先序遍历和中序遍历建立二叉树
template <typename T>void Post<T>::CreateBT3(BTNodeClass<T> &obj,T *poststr,T *instr,int n)
{
	obj.r = CreateBT31(poststr,0,instr,0,n);
}
template <typename T>BTNode<T> * Post<T>::CreateBT31(T *poststr,int ipost,T *instr,int iin,int n)
{
	BTNode<T> *t;
	char ch;
	int p,k;
	if(n <= 0) return NULL;
	ch = poststr[ipost+n-1];
	t = new BTNode<T>;
	t->data = ch;
	p = iin;
	while(p <iin+n)
	{
		if(instr[p] == ch)
		{
			break;
		}
		p++;
	}
	k = p - iin;
	t->lchild = CreateBT21(poststr,ipost,instr,iin,k);
	t->rchild = CreateBT21(poststr,ipost+k,instr,p+1,n-k-1);
	return t;
}
//利用先序遍历和中序遍历建立二叉树
template <typename T>void Post<T>::CreateBT2(BTNodeClass<T> &  ,T *prestr,T *instr,int n)
{
	obj.r = CreateBT21(prestr,0,instr,0,n);
}
template <typename T>BTNode<T> * Post<T>::CreateBT21(T *prestr,int ipre,T *instr,int iin,int n)
{
	BTNode<T> *t;
	char ch;
	int p,k;
	if(n <= 0) return NULL;
	ch = prestr[ipre];
	t = new BTNode<T>;
	t->data = ch;
	p = iin;
	while(p <iin+n)
	{
		if(instr[p] == ch)
		{
			break;
		}
		p++;
	}
	k = p - iin;
	t->lchild = CreateBT21(prestr,ipre+1,instr,iin,k);
	t->rchild = CreateBT21(prestr,ipre+k+1,instr,p+1,n-k-1);
	return t;
}
//输出所有的祖先节点。运用队列；
template <typename T>bool Post<T>::Ancestor32(BTNodeClass<T> &obj,T x)
{
	return Ancestor322(obj.r,x);
}
template <typename T>bool Post<T>::Ancestor322(BTNode<T> *t,T x)
{
	BTNode<T> *p;
	QUType<T> qu[MAXSIZE];//定义非循环队列
	int front,rear,i;
	front = rear = -1;//当都为-1时表示队列为空
	rear++;qu[rear].node = t;//将头节点进队列
	qu[rear].parent = -1;//将双亲节点置为一
	while(front != rear)//如果队列中还有元素，则继续循环
	{
		//rear记录了们一个元素的下标
		front++;p = qu[front].node;//出对列
		//front不仅仅记录了们一个元素进队列时的下标，而且当有元素进队列时，他将父节点的下标给了子节点
		//让子节点记录父节点的下标，当输出祖先节点时，我们只需要寻找当前节点的parent，而如果把当前节点的parent
		//当作原先队列的下标，就可以找到他的父节点，以此类推

		if(p->data == x)//判断是否相等
		{
			i = qu[front].parent;
			if(i != -1)
			{
				while(qu[i].parent != -1)//打印它的祖先节点，除了根节点
				{
					cout<<qu[i].node->data<<" ";
					i = qu[i].parent;
				}
				cout<<qu[i].node->data;//打印根节点，因为当上面的i=0时，根节点的parent已经变成了-1，不符合条件，退出了循环
				//而根节点也是这个二叉树的祖先节点，所以打印它
				return true;
			}
		}
		//当有元素时，让左右子树进队列
		if(p->lchild != NULL)
		{
			rear++;qu[rear].node = p->lchild;
			qu[rear].parent = front;//将父亲节点的下标赋给左右子树parent；
		}
		if(p->rchild != NULL)
		{
			rear++;qu[rear].node = p->rchild;
			qu[rear].parent = front;
		}
	}
	return false;
}
//采用队列的思想进行层次遍历
template <typename T>void Post<T>::LevelOrder1(BTNodeClass<T> &obj)
{
	LevelOrder11(obj.r);
}
template <typename T>void Post<T>::LevelOrder11(BTNode<T> *t)
{
	BTNode<T> *qu[MAXSIZE];
	int rear = 0,front = 0;//定义两个临时下标
	rear = (rear+1)%MAXSIZE;//蒋将数据加一
	qu[rear] = t;//将头节点进队
	while(rear != front)
	{
		front = (front+1)%MAXSIZE;//什么是队列，队列就是先进先出，如果是数组他也可以模拟一个队列
		//我们只需要给他一个最大值取余就可以
		cout<<qu[front]->data<<" ";//输出最先进来的人
		//如果有，就进队列，将下标后移
		if(qu[front]->lchild != NULL)
		{
			rear = (rear + 1)%MAXSIZE;
			qu[rear] = qu[front]->lchild;
		}
		//如果有，就进队列，将下标后移
		if(qu[front]->rchild != NULL)
		{
			rear = (rear + 1)%MAXSIZE;
			qu[rear] = qu[front]->rchild;
		}
	}
}
//采用后续遍历的算法，实现对祖先节点的输出
template <typename T>bool Post<T>::Ancestor31(BTNodeClass<T> &obj,T x)
{
	return Ancestor311(obj.r,x);
}
template <typename T>bool Post<T>::Ancestor311(BTNode<T> *t,T x)
{
	BTNode <T> * buf[MAXSIZE];
	BTNode<T> *p = t,*q;
	int top = -1;
	int i = 0;
	bool flag;
	do 
	{
		while(p != NULL)
		{
			top++;buf[top] = p;
			p = p->lchild;
		}
		flag = true;
		q = NULL;
		while (top != -1 && flag == true)//只有两个条件同时成立，才能往下进行
		{
 			if(p->rchild == q)
			{
				if(p->data == x)//判断如果是要找的节点，则输出器祖先节点
				{
					for (i = 0; i< top; i++)
					{
						cout<<buf[i]->data<<" ";
					}
					return true;//找到返回true；
				}
				else
				{
					top--;//如果没有找到，则退栈
					q = p;
				}
			}
			else 
			{
				p = p->rchild;
				flag = false;
			}
		}
	} while (top != -1);
	return false;//找不到返回false
}
//后续遍历的非递归算法
template <typename T>void Post<T>::NPostOrder1(BTNodeClass<T> &obj)
{
	NPostOrder11(obj.r);
}
template <typename T>void Post<T>::NPostOrder11(BTNode<T> *t)
{//{
	BTNode<T> *buf[MAXSIZE];
	BTNode<T>* p = NULL;
	int top = -1;
	p = t;
	//后续遍历算法
	bool flag;//设立标志位，用于判断左子树是否为空
	BTNode<T> *q;
	do 
	{
		while(p != NULL)//判断左子树是否为空
		{
			top++;buf[top] = p;//将左子树全部入栈
			p = p->lchild;
		}
		q = NULL;//这个用于判断右子树是否为NULL
		flag = true;//设立标志位，标志左子树已经为空
		while(top != -1 && flag == true)//判断栈是否为空，并且左子树已经为空
		{
			p = buf[top];//将栈中的元素取出来，赋给p，top--不能写到这里
			//因为，如果当前节点的右子树与上一次退栈的节点相同时
			//我没才需要进行退栈，

			if(p->rchild == q)//判断右子树是否为空
			{
				cout<<p->data<<" ";//打印的是没有左子树和右子树的节点
				top--;
				q = p;//将上一次打印的节点记录下来
			}else
			{
				p = p->rchild;//将指针下移
				flag = false;//将标志位设置为false，意思是左子树不为空
				//并且将循环移出，重复do while循环,让他继续判断右子树的左子树是否为空
			}
		}
	} while (top != -1);//判断栈是否为空

}
//中序遍历的非递归算法
template <typename T>void Post<T>::NInOrder1(BTNodeClass<T> &obj)
{
	NInOrder11(obj.r);
}
template <typename T>void Post<T>::NInOrder11(BTNode<T> *t)
{
	BTNode<T> *buf[MAXSIZE];
	BTNode<T> *p = NULL;
	p = t;
	int top = -1;
	while(top != -1 || p != NULL)//判断栈是否为空
	{
		while(p != NULL)//判断左子树是否为空
		{
			top++;buf[top] = p;//如果不为空，则进栈
			p = p->lchild;
		}
		if(top != -1)//判断栈中是否有元素
		{
			p = buf[top];top--;//得到没有左子树为空的父亲节点
			cout<<p->data<<" ";//输出当前节点
			p = p->rchild;//找当前节点的右子树，将右子树赋给p，再对他进行判断
		}
	}
}
//前向非递归算法
template <typename T>void Post<T>::NPreOrder2(BTNodeClass<T> &obj)
{
	NPreOrder22(obj.r);
}
template <typename T>void Post<T>::NPreOrder22(BTNode<T> *t)
{
	BTNode<T> *buf[MAXSIZE];
	int top = -1;
	BTNode<T>* p = NULL;
	p = t;
	while(top != -1 || p != NULL)//在这里虽然top != -1 不成立，但后者成立导致了程序的自主运行
	{
		while(p != NULL)//判断左子树是否为空
		{
			cout<<p->data<<" ";//输出父节点
			top++;buf[top] = p;//将父节点进栈
			p = p->lchild;
			//寻找下一个左子树======>如果已经将左子树查找完毕之后，p= NULL;所以下面必须将buf中的元素几进行出栈
		}
		if(top != -1)
		{
			p = buf[top];top--;//对他进行出栈操作
			p = p->rchild;//判断右子树
			//当这一句话执行结束后，它会跳出去进行判断，当前p就成了父节点
		}
	}
}
template <typename T>void Post<T>::NPreOrder1(BTNodeClass<T> &obj)
{
	NPreOrder11(obj.r);
}
template <typename T>void  Post<T>::NPreOrder11(BTNode<T> *t)
{
	BTNode<T> *buf[MAXSIZE];//定义一个顺序栈
	int top = -1;
	BTNode<T> *p = NULL;//定义一个指针类型，用来遍历所有节点
	top++;
	buf[top] = t;//将头节点给给你str[top]；他的目的在循环中
	//在另一个角度看，也就是头节点进栈
	while(top != -1)
	{
		p = buf[top];top--;//p始终是出栈的元素
		cout<<p->data<<" ";
		if(p->rchild != NULL)//如果右子树不为空则让右子树进栈
		{
			top++;
			buf[top] = p->rchild;//进栈操作
		}
		if(p->lchild != NULL)//如果左子树不为空则让右子树进栈
		{
			top++;
			buf[top] = p->lchild;//进栈操作
		}
	}
}
void main()
{
	BTNodeClass<char> mych; 
	BTNodeClass<char> pch;
	Post<char> q2;
	char *prestr = "ABDGCEF";
	char *instr = "DGBAECF";
	char *poststr = "GDBEFCA";
	int len = strlen(prestr);
	char *mystr = "A(B(D(g,f(,G)),K(L,S)),C(E,F))";
	cout<<len<<endl;
	BTNode<char> *temp = NULL;
	mych.CreatBTNode(mystr);
	cout<<"前序遍历如下"<<endl;
	q2.NPreOrder1(mych);
	cout<<"\n前序遍历如下"<<endl;
	q2.NPreOrder2(mych);
	cout<<"\n中序遍历结果如下："<<endl;
	q2.NInOrder1(mych);
	cout<<"\n后续遍历结果如下："<<endl;
	q2.NPostOrder1(mych);
	cout<<endl;
	cout<<"所有的祖先节点是："<<endl;
	q2.Ancestor31(mych,'g');
	cout<<"\n采用队列的思想进行层次遍历"<<endl;
	q2.LevelOrder1(mych);
	cout<<"\n采用层次遍历的方法遍历所有的祖先节点"<<endl;
	q2.Ancestor32(mych,'G');
	cout<<endl;
	q2.CreateBT2(pch,prestr,instr,len);
	cout<<"\n前序遍历如下"<<endl;
	q2.NPreOrder2(pch);
	cout<<"\n中序遍历结果如下："<<endl;
	q2.NInOrder1(pch);
	q2.CreateBT3(pch,poststr,instr,len);
	cout<<"\n前序遍历如下"<<endl;
	q2.NPreOrder2(pch);
	cout<<"\n中序遍历结果如下："<<endl;
	q2.NInOrder1(pch);
	system("pause");
}
