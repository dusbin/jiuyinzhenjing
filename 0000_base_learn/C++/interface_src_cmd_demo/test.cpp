/**
  https://www.cnblogs.com/MakeView660/p/6038129.html
 */
#include <iostream>
#include <string.h>
using namespace std;
//using std::string;
/**
 纯虚函数语法：
 　	将成员函数声明为virtual
	后面加上 = 0
	该函数没有函数体
class <类名>
{
    virtual <类型><函数名>(<参数表>) = 0;
    …
};
 基类中不能对虚函数给出有意义有实现，而把它说明为纯虚函数，它的实现留给该基类的派生类去做。这就是纯虚函数的作用
 抽象类
　　含有纯虚函数的类叫做抽象类（纯虚类），抽象类是一种特殊的类，它是为了抽象和设计的目的而建立的，它处于继承层次结构的较上层。
　　抽象类不能被实例化，即无法创建该类的对象。
　　CmdHandler ch;                    // 编译错误！！
　　CmdHandler *p = new CmdHandler();  // 编译错误！！
　　在实际中为了强调一个类是抽象类，可将该类的构造函数说明为保护的访问控制权限。
　　抽象类的主要作用是将有关的组织在一个继承层次结构中，由它来为它们提供一个公共的根，相关的子类是从这个根派生出来的。
    抽象类刻画了一组子类的操作接口的通用语义，这些语义也传给子类。一般而言，抽象类只描述这组子类共同的操作接口，而完整的实现留给子类。
	抽象类只能作为基类来使用，其纯虚函数的实现由派生类给出。如果派生类没有重新定义纯虚函数，而派生类只是继承基类的纯虚函数，则
	这个派生类仍然还是一个抽象类。如果派生类中给出了基类纯虚函数的实现，则该派生类就不再是抽象类了，它是一个可以建立对象的具体类了。
 接口
　　实际用途：充当“接口函数”
　　(相当于Java中的interface语法)
　　(用于替代C中的回调函数的用法)
　　接口规范：凡是遵循此规范的类，都必须实现指定的函数接口，通常是一系列接口。
　　上述定义的抽象类可以理解为：凡是遵循CmdHandler规范的类，都必须实现指定的函数接口：OnCommand()。
 需求：用户输入一行命令，按回车完成输入。要求解析命令的输入，并处理之。
 设计：
	CmdInput：用于接收用户的输入
	CmdHandler：规定一系列函数接口
　　CmdParser：接口的实现，实际用于解析的处理类
 说明：    	
	可见，CmdInput只接收输入的内容，而CmdParser用于对输入的内容进行解析，两个类各做各的，互不干涉，两者并不知道对
	方的存在，而是通过抽象类CmdHandler充当“接口”联系起来。
　　1、  纯虚函数的定义
　　2、  抽象类及其实质作用：接口规范，因为它只代表了一个规范，并没有具体实现，所以它不能被实例化。
　　3、  抽象类通常被多重继承，例如，一个普通的类，实现了多套接口规范，又继承于原有的父类。
　　4、  抽象类的析构函数应该声明为virtual，因为它被涉及用于继承的。
 */
class CmdHandler{
public:
	virtual ~CmdHandler(){} //析构函数声明为virtual
	virtual void OnCommand(char* cmdline) = 0;
};
class CmdInput{
public:
	CmdInput();
	void SetHandler(CmdHandler* pCHander);
	int Run();
private:
	CmdHandler* m_pCHandler;
};
CmdInput::CmdInput(){
	m_pCHandler = NULL;
}
void CmdInput::SetHandler(CmdHandler* pCHander){
	m_pCHandler = pCHander;
}
int CmdInput::Run(){
	char cmdline[256];
	memset(cmdline,0x00,256);
	while(1){
		cout<<"> ";//输入
		cin>>cmdline;
		if(strcmp(cmdline,"exit") == 0){
			break;
		}
		if(m_pCHandler){
			m_pCHandler->OnCommand(cmdline);
		}
	}
	return 0;
}
class CmdParser:public CmdHandler{
public:
	//CmdParser();
	virtual void OnCommand(char* cmdline);//函数接口集
private:
	int Split(char text[],char* parts[]);//解析命令
};
void CmdParser::OnCommand(char* cmdline){
	char* argv[128];
	int argc = Split(cmdline,argv);
	cout<<"argc = "<<argc<<endl;
	if(argc > 0){
		cout<<"命令："<<argv[0]<<endl;
		cout<<"参数："<<endl;
		for(int i = 1;i < argc;i++){
			cout<<argv[i]<<" ";
		}
		cout<<endl;
	}
}
int CmdParser::Split(char text[],char* parts[]){
	int count = 0;
	int start = 0;
	int flag = 0;
	int stop = 0;
	for(int i = 0;!stop;i++){
		char ch = text[i];
		if(ch == 0)
			stop = 1;
		if(ch == ','||ch == '\0'||ch == ' '||ch == '\t'){
			if(flag){
				flag = 0;
				text[i] = 0;
				parts[count] = text + start;
				cout<<"parts["<<count<<"]:"<<parts[count]<<endl;
				count++;
			}
		}else{
			if(!flag){
				flag = 1;
				start = i;
			}
		}
	}
	return count;
}
void test_01(){
	CmdInput CInput;
	CmdParser CParser;
	CInput.SetHandler(&CParser);
	CInput.Run();
}
int main(){
	test_01();
	return 0;
}


