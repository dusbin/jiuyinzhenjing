#ifndef _THREADSTRUCT_H_
#define _THREADSTRUCT_H_

#define MAXSIZE 20
template <typename T>struct BthNode
{
	T data;
	BthNode * lchild,*rchild;
	int rtag,ltag;
};
template <typename T>class BthNodeClass
{
	BthNode<T> *r;
	BthNode<T> *root;
	BthNode<T> *pre;
public:
	BthNodeClass();
	~BthNodeClass();
	void CreateBTNode(char *str);
	void DispBTNode();
	void CreateThread();
	void ThInOrder();
	//void DestoryBTree1();
	void IOthDNode(int dir,T nData,int inData);//中序遍历线索化删除节点
private:
	void IsertRight(BthNode<T> **nData, BthNode<T>**inData);//将节点inData插入到nData右侧
	void IsertLeft(BthNode<T> *&nData, BthNode<T>* &inData);//将节点inData插入到nData左侧
	void DestoryBTree1();
	void DestoryBTree1(BthNode<T> *t);
	void DispBTNode1(BthNode<T> *t);
	void Thread(BthNode<T> *t);
protected:
};     


#endif