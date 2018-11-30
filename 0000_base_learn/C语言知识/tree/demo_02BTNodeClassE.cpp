#include <iostream>
#include "demo01_BTNodeClass.cpp"
using namespace std;

//求一个算法，求二叉树中节点个数
template <typename T>
class A
{
	//********************************************递归算法******************************************/
public:
	int NodeCount1(BTNodeClass<T> &obj);
	int NodeCount2(BTNodeClass<T> &obj);
	void DispLeaf(BTNodeClass<T> &obj);
	void CopyTree(BTNodeClass<T> &obj_0,BTNodeClass<T> &obj_1);
	//void Trans(char sqbstree[],BTNodeClass<T> &obj);

	int level(BTNodeClass<T> &obj,T x);
	bool Ancestor(BTNodeClass<T> &obj,T x);//寻找x节点的所有节点


private:
	int NodeCount11(BTNode<T> *t);
	int NodeCount21(BTNode<T> *t);
	void DispLeaf1(BTNode<T> *t);
	void CopyTree1(BTNode<T> *t_0,BTNode<T> *t_1);
	//void Trans1(char sqbstree[],int i,BTNode<T> *&t);//这是一个指针，一个BTNode类型的指针，&t只不过时一个变量名
	//他的意思是，给传进来的变量重新起了一个名字，没有其他特殊的意义，
	int level1(BTNode<T> *t,T x,int H);
	bool Ancestor1(BTNode<T> *t,T x);//寻找x节点的所有节点
	/******************************************以上为递归算*************************************************/

};

//*****************************************一下为二叉树的递归算法******************************************/
template <typename T>
//先序遍历
int A<T>::NodeCount1(BTNodeClass<T> &obj)
{
	return NodeCount11(obj.r);
}
template <typename T>int A<T>::NodeCount11(BTNode<T> *t)
{
	int m,n,k;
	if(t != NULL)
	{
		k = 1;//先序遍历
		m = NodeCount11(t->lchild);
		//k = 1;中序遍历
		n = NodeCount11(t->rchild);
		//k = 1;后序遍历
		return k+m+n;
	}
	else return 0;
}


//完全利用递归得特性，来解决计算子叶的问题
template <typename T>int  A<T>::NodeCount2(BTNodeClass<T> &obj)
{
	return NodeCount21(obj.r);
}
template <typename T>int  A<T>::NodeCount21(BTNode<T> *t)
{
	if(t == NULL) return 0;
	else 
		return (NodeCount21(t->lchild)+NodeCount21(t->rchild)+1);
}


//输出子叶，通过递归的方法
template <typename T>void  A<T>::DispLeaf(BTNodeClass<T> &obj)
{
	return DispLeaf1(obj.r);
}
template <typename T>void  A<T>::DispLeaf1(BTNode<T> *t)
{
	if(t != NULL)
	{
		if(t->lchild == NULL && t->rchild == NULL)//如果左子树和右子树都为空，则说明他是一个子叶
			cout<<t->data<<" ";
		DispLeaf1(t->lchild);//递归在左子树中寻找子叶
		DispLeaf1(t->rchild);
	}
}

template <typename T>void  A<T>::CopyTree(BTNodeClass<T> &obj_0,BTNodeClass<T> &obj_1)
{
	CopyTree1(obj_0.r,obj_1.r);
}
template <typename T>void  A<T>::CopyTree1(BTNode<T> *t_0,BTNode<T> *t_1)
{
	if(t_0 != NULL)
	{
		t_1 = new BTNode<T>();
		t_1->data = t_0->data;
		CopyTree1(t_0->lchild,t_1->lchild);
		CopyTree1(t_0->rchild,t_1->rchild);
	}
	else t_1 = NULL;
}
template <typename T>int A<T>::level(BTNodeClass<T> &obj,T x)//输出x所在二叉树的层次（H）
{
	return level1(obj.r,x,1);//1是一个初始值
}
template <typename T>int A<T>::level1(BTNode<T> *t,T x,int H)//H在每次调用时都发生了变化，即下一层的值
{
	int l;
	if(t == NULL) return 0;
	else if(t->data == x) return H;
	else 
	{
		l = level1(t->lchild,x,H+1);//查找左子树，如果在，则返回其层次
		if(l != 0) return l;
		else return (level1(t->rchild,x,H+1));//查找左子树
	}
}
template <typename T>bool A<T>::Ancestor(BTNodeClass<T> &obj,T x)//寻找x节点的所有节点
{
	return Ancestor1(obj.r,x);
}
template <typename T>bool A<T>::Ancestor1(BTNode<T> *t,T x)//寻找x节点的所有节点      
{	//如果当前节点为空，则失败
	if(t == NULL) return false;
	//如果x与左子节点的值刚好相等，则输出当前节点
	if(t->lchild != NULL && t->lchild->data == x)
	{
		cout<<t->data<<" ";
		return true;
	}
	//如果x与右子节点的值刚好相等，则输出当前节点
	if(t->rchild != NULL && t->rchild->data == x)
	{
		cout<<t->data<<" ";
		return true;
	}
	//如果没有在第二层找到，则继续递归寻找
	//由于递归的特性可以得到，他输出时刚好是一个逆过程
	//也就是说，当递归的找到元素时，递归会退出来，刚好逆序的退出来，显示出结果
	if(Ancestor1(t->lchild,x) || Ancestor1(t->rchild,x))
	{
		cout<<t->data<<" ";
		return true;
	}else
		return false;
}
//void main21()
//{
//	btnodeclass<char> mych,copymych; 
//	char *mystr = "a(b(d(g,f(,g)),),c(e,f))";
//	char sqbstree[] = "abc###d##";
//
//	btnode<char> *temp = null;
//	mych.creatbtnode(mystr);
//	cout<<"前序遍历如下"<<endl;
//	mych.dispbtnode();
//	mych.inorder(mych);
//	cout<<endl;
//	a<char> q1;
//	cout<<q1.nodecount1(mych);//先序遍历
//	cout<<endl;
//	cout<<q1.nodecount2(mych);//利用递归特性，来计算子夜问题
//	cout<<endl;
//	q1.displeaf(mych);//输出子叶，通过递归得方法
//	cout<<endl;
//	q1.copytree(mych,copymych);
//	cout<<"copy 的树如下："<<endl;
//	mych.dispbtnode();
//	mych.inorder(mych);
//	cout<<endl;
//
//	//q1.trans(sqbstree,mych);
//	cout<<q1.level(mych,'g');
//	cout<<endl;
//	q1.ancestor(mych,'g');
//	system("pause");
//}