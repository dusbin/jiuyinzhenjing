volatile 关键字和const对应。
const是不变的，
而 volatile 是可以被编译器未知的因素改变，比如操作系统，硬件，其他线程。遇到这个声明的变量，编译器对访问该变量
的代码不进行优化，从而提供特殊地址的稳定访问
当要求使用volatile声明的变量时，系统总是从它所在的内存读取数据，即使它前面的指令刚刚从该处读取过数据，而且读取的数据立刻被保存
如
	volatile int i = 10;
	int a = i;
	//其他代码，未明确告诉编译器，对i进行过操作
	int b = i;
volatile 指出i是随时可能发生变化的，每次使用它的时候必须从i的地址中读取，因而编译器生成的汇编代码会重新从i的地
址读取数据放在b中
而优化做法是，由于编译器发现两次从 i读数据的代码之间的代码没有对 i 进行过操作，
它会自动把上次读的数据放在 b 中。而不是重新从 i 里面读。这样以来，如果 
i是一个寄存器变量或者表示一个端口数据就容易出错，所以说 volatile 可以保证对特殊地址的稳定访问。
输入下面的代码：
#include <stdio.h>
void main()
{
	int i = 10;
	int a = i;
	printf("i = %d", a);
	// 下面汇编语句的作用就是改变内存中 i 的值
	// 但是又不让编译器知道
	__asm{
		mov dword ptr [ebp-4], 20h
	}	 
	int b = i;
	printf("i = %d", b);
}
然后，编译时将优化选项关闭，运行程序，输出结果如下：
i = 10
i = 32
然后，编译时将优化选项开启，运行程序，输出结果如下：
i = 10
i = 10
输出的结果明显表明，编译时将优化选项开启下，编译器对代码进行了优化，第二次没有输出正确的 i 值。下面，我们把 i 的声明加上 volatile 关键字，看看有什么变化：
#include <stdio.h>
void main()
{
	volatile int i = 10;
	int a = i;
	printf("i = %d", a);
	__asm {
		mov dword ptr [ebp-4], 20h
	}
	int b = i;
	printf("i = %d", b);
}
分别在 编译时将优化选项开启和关闭运行程序，输出都是：
i = 10
i = 32
这说明这个 volatile 关键字发挥了它的作用。其实不只是“内嵌汇编操纵栈”这种方式属于编译无法识别的变量改变。
另外更多的可能是多线程并发访问共享变量时，一个线程改变了变量的值，怎样让改变后的值对其它线程 visible。一般说来，volatile用在如下的几个地方：
中断服务程序中修改的供其它程序检测的变量需要加volatile；
多任务环境下各任务间共享的标志应该加volatile；
存储器映射的硬件寄存器通常也要加volatile说明，因为每次对它的读写都可能由不同意义；
2.volatile 指针
和 const 修饰词类似，const 有常量指针和指针常量的说法，volatile 也有相应的概念：
修饰由指针指向的对象、数据是 const 或 volatile 的：
const char* cpch;
volatile char* vpch;
指针自身的值——一个代表地址的整数变量，是 const 或 volatile 的：
char* const pchc;
char* volatile pchv;
注意：
 可以把一个非volatile int赋给volatile int，但是不能把非volatile对象赋给一个volatile对象。
 除了基本类型外，对用户定义类型也可以用volatile类型进行修饰。
C++中一个有volatile标识符的类只能访问它接口的子集，一个由类的实现者控制的子集。用户只能用const_cast来获得对类型接口的完全访问。此外，volatile向const一样会从类传递到它的成员。
3. 多线程下的volatile
有些变量是用volatile关键字声明的。当两个线程都要用到某一个变量且该变量的值会被改变时，应该用volatile声明，该关键字的作用是防止优化编译器把变量从内存装入CPU寄存器中。
如果变量被装入寄存器，那么两个线程有可能一个使用内存中的变量，一个使用寄存器中的变量，这会造成程序的错误执行。volatile的意思是让编译器每次操作该变量时一定要从内存中真正取出，而不是使用已经存在寄存器中的值，如下：
 volatile BOOL bStop = FALSE; 
 在一个线程中：
while( !bStop ) { ... }
bStop = FALSE;
return;
在另外一个线程中，要终止上面的线程循环：
bStop = TRUE;
while( bStop ); 
等待上面的线程终止，如果bStop不使用volatile申明，那么这个循环将是一个死循环，因为bStop已经读取到了寄存器中，寄存器中bStop的值永远不会变成FALSE，加上volatile，程序在执行时，每次均从内存中读出bStop的值，就不会死循环了。
这个关键字是用来设定某个对象的存储位置在内存中，而不是寄存器中。因为一般的对象编译器可能会将其的拷贝放在寄存器中用以加快指令的执行速度，例如下段代码中：
 ... 
 int nMyCounter = 0; 
 for(; nMyCounter<100;nMyCounter++) 
 { 
 ... 
 } 
 ...
在此段代码中，nMyCounter的拷贝可能存放到某个寄存器中（循环中，对nMyCounter的测试及操作总是对此寄存器中的值进行），但是另外又有段代码执行了这样的操作：nMyCounter -= 1;这个操作中，对nMyCounter的改变是对内存中的nMyCounter进行操作，于是出现了这样一个现象：nMyCounter的改变不同步。