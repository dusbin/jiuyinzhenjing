type mytype int //定义新的类型 即别名
func (p mytype) funcname(q int)(r,s int) {return 0,0}
----  --------  -------- -----  -------   ----------
 0        1         2      3       4          5
0 保留字func用于定义一个函数
1 函数可以定义用于特定的类型，这类函数更加通俗的称呼是method，这部分称作receiver，是可选的
2 函数名
3 int是变量q的输入参数。参数用pass-by-value传递，意味着他们会被复制
4 r s 返回值，go语言中可以有多个返回值。如果不想对然悔之命名，可以只提供类型(int,int),只有一个返回值时可以省略圆括号，如果函数是一个子过程，并且没有任何返回值，也可以省略这些内容
5 函数体，
如：
	//没有返回值
	func subroutine(in int){
		return
	}
	//将输入返回
	func identity(in int) int{
		return
	}
函数不需要声明，可以随意安排函数定义顺序，编译器会在执行前扫描每个文件。
go不允许函数嵌套，可以利用匿名函数实现。递归函数和其他是一样的
变量的作用域
package main
var a = 6
func main(){
	p()
	q()
	p()
}
func p(){
	println(a)
}
func q(){
	a := 5//重新定义了一个局部的a
	println(a)//34行的定义，
}
package main
var a = 6
func main(){
	p()
	q()
	p()
}
func p(){
	println(a)
}
func q(){
	a = 5//复制，还是38行的a
	println(a)
}
多值返回
命令返回值
go函数的返回值或者结果参数可以指定一个名字，并且像原始的变量那样使用，就像输入参数那样
如果对其命名，在函数开始时，会进行零值初始化，如果函数不加参数的情况下执行了return语句，
结果参数的当前值会作为返回值返回。 允许用较少的代码做更多的事
名字不是强制，但是可以使代码更加健壮和清晰
func nextInt(b []byte,pos int)(value,nextpos int){/**...*/}
io.ReadFull的实现代码
func ReadFull(r read,buf []byte)(n int,err error){
	for len(buf) > 0 && err == nil {
		var nr int
		nr,err = r.Read(buf)
		n+=nr
		buf = buf[nr:len(buf)]
	}
	return
}
延迟代码
假设有一个函数，打开文件并且对其进行若干读写，这样的函数中经常有提前返回的地方，
如果这样做需要关闭正在工作的文件描述符，这经常大致产生下面的代码
存在很多重复的代码
func ReadWrite() bool{
	file.Open("file")
	if failurex {
		file.Close()
		return false
	}
	if failurey{
		file.Close()
		return false
	}
	file.Close()
	return true
}
为接解决这个问题，go语言有defer语句，在defer后制定的函数会在函数退出前调用。
将Close对应的放在Open后，能够是函数更加可读健壮
延迟处理会按照后进先出的顺序执行
func ReadWrite() bool{
	file.Open("file")
	defer file.Close() //函数结束时会调用
	if failurex {
		return false //会调用89行
	}
	if failurey{
		return false //会调用89行
	}
	return true //会调用89行
}
defer func(){
	/* ... */
}() //()在这里是必须的
或者这个例子，更加容易了解为什么，以及在哪里需要括号：
带参数的函数符号
defer func(x int){
	/* ... */
}(5) //为输入参数x赋值5
在这个（匿名）函数中，可以访问任何命名返回参数：
在defer中访问返回值
func f() (ret int){ //ret初始化为零
	defer func(){
		ret++ //ret增加为1
	}()
	return 0 //返回的是1而不是0！
}
变参
接受变参的函数是有着不定数量的参数的，为了做到这点，首先需要定义函数使其接受变参
func myfunc(arg ...int){}
arg ...int告诉go这个函数接受不定数量的参数，这些参数的类型全部是int，函数体中，
变量arg是一个int类型的slice
for _,n:=range arg{
	fmt.Println("And the number is :",n)
}
如果不指定变参的类型，默认是空的接口interface{} 假设另外一个变参函数叫做myfunc2
func myfunc(arg ...int){
	myfunc2(arg...)//按原样传递
	myfunc2(arg[:2]...)//传递部分
}
函数作为值
func main{
	a:=func(){ //定义一个匿名函数，赋值给a
		fmt.Println("hello")
	}//没有()
	a()
}
xs:= map[int]func() int{
	1:func() int{return 10},
	2:func{} int{return 20},
	3:func() int{return 30},//必须有逗号
}
回调
当函数作为值时，就可以很容易的传递到其他函数里，然后可以作为回调，首先定义一个函数，对整数做一些事情
func printit(x int){//无返回值
	fmt.Println(x)
}
这个函数的表示是func printit(int),或者没有函数名的，func(int).创建新的函数使用这个作为回调
func callback(y int,f func(int)){//f将会保存函数
	f(y) //调用回调函数f输入变量y
}
恐慌和恢复
go语言没有异常机制不能抛出异常，作为替代 引入了恐慌和恢复。
这应该是最后的一道防护。代码中应该没有或者很少有令人恐慌的东西
Panic
是一个内建函数，可以中断原有的控制流程，进入一个令人恐慌的流程中。当函数F调用panic，函数F的
执行被中断，并且F中的延迟函数会正常执行，然后F返回到调用它的地方。在调用的地方，F的行为就像
调用了panic。这一过程继续向上，直到程序崩溃时的所有goroutine返回。恐慌可以直接调用panic产生。
也可以由运行时错误产生，例如访问越界的数组。
Recover
是一个内建的函数，可以让进入令人恐慌的流程中的goroutine恢复过来。recover仅在延迟函数中有效。在正常的执行过程中，调用recover会返回
nil并且没有其他任何效果。如果当前的goroutine陷入恐慌，调用recover可以捕获到panic的输入值，并且恢复正常的执行。	