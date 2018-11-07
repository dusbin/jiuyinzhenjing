package main
import "fmt"
func main(){
	fmt.Printf("Hello, world;orこんにちは;or世界，你好\n")
	a := 15
	b := false
	//fmt.Printf("a: ",a," b: ",b)//也可以使用，不过和Println打印的不一样，存在差异，Println才是想要的
	fmt.Println(" a: ",a," b: ",b)//15 false
	var int_a int 
	/**
		int8 int16 int32 int64 byte(uint8的别名) uint8 uint16 uint32 uint64 float32 float64,与cpu架构无关，与64位32位无关
		混合使用会引起编译器错误。即 int16 int8不能相加
		赋值可以使用八进制十六进制或者科学计数法，077 0xFF 1e3 6.022e23
	 */
	int_a = 123
	fmt.Println("int_a: ",int_a)//123
	const const_x = 42 
	//生成x这个常量，也可以使用iota生成枚举值
	fmt.Println("const_x :",const_x)//42
	//const_x = 43//不能修改，编译器报错
	//生成x这个常量，也可以使用iota生成枚举值
	fmt.Println("const_x :",const_x)//42
	const(
		const_a = iota //使用iota生成枚举值 第一个0 第二个1
		const_b = iota
	)
	fmt.Println("const_a :",const_a)//0
	fmt.Println("const_b :",const_b)//1
	const(
		const_a1 = iota //使用iota生成枚举值 第一个0 第二个1
		const_b1 		//自动自增
		const_c1
	)
	fmt.Println("const_a1 :",const_a1)//0
	fmt.Println("const_b1 :",const_b1)//1
	fmt.Println("const_c1 :",const_c1)//2
	const(
		const_a2 = iota //使用iota生成枚举值 第一个0 第二个1
		const_b2 = 0		//自动自增
		const_c2 string = "a"
	)
	fmt.Println("const_a2 :",const_a2)//0
	fmt.Println("const_b2 :",const_b2)//0
	fmt.Println("const_c2 :",const_c2)//"a"
	s := "Hello kitty!" //一旦赋值，字符串就不能修改了，go语言中字符串不可改变，
	fmt.Println("s:",s)//Hello kitty!
	//s[1] = 'c' //非法的修改数组第一个字符
	c:=[]byte(s) //转换为字节数组c
	c[0] = 'c'//修改数组第一个元素
	s2 :=string(c)//创建新的字符串s2保存修改。
	fmt.Println("s2:",s2)//cello kitty!
	//多行字符写法
	/**
	 错误写法
	 s:= "Staring part";
		+ "Ending part";
	 */
	s_more_line := "Staring Part" + 
		"Ending Part"
	fmt.Println("s_more_line: ",s_more_line)
	//使用反引号`
	s_more_line1 := `Staring Part
Ending Part`
	fmt.Println("s_more_line1: ",s_more_line1)
	//rune 是int32的别名，使用utf-8进行编码时，遍历字符串的字符，可以循环每个字节（仅使用US ASCII编码）
	//字符串时与字符等价，为了获取实际的字符，需要使用rune类型
	/**
	 go语言支持复数，complex128(64位虚数部分) complex64(32位虚数部分)
	 复数写为re+imi re为实数部分，im为虚数部分。i是标记虚数部分
	 */
	complex_c := 5+5i
	complex_b := 5+5i
	fmt.Println("complex_c:",complex_c)//5+5i
	fmt.Println("complex_c+complex_b:",complex_c+complex_b)//10+10i
	fmt.Println("complex_c-complex_b:",complex_c-complex_b)//0+0i
	fmt.Println("complex_c*complex_b:",complex_c*complex_b)//0+50i
	fmt.Println("complex_c/complex_b:",complex_c/complex_b)//1+0i
	//错误 error
	/**
	 运算符
	 +-*、%和c语言类似
	 & 按位与 | 按位或 ^ 按位异或 &^ 位清除
	 && || 逻辑与 逻辑或
	 go语言不支持运算符重载(或者方法重载),而一些内建运算符支持重载，如+可以用于整数，浮点数，复数，字符串
	 */
	/**
		go保留字
		break		default 	func	interface	select
		case		defer		go		map			struct
		chan		else		goto	package		switch
		const		fallthrough	if		range		type
		continue	for			import	return		var
	 */
	 /**
	  go控制结构
		循环 for 没有do while
		switch if select 
		语法无需圆括号，语句整体必须包含在大括号内
		if x > 0 {
			return y
		}else{
			return x
		}
		强制大括号鼓励将简单的if语句写在多行上。无论如何，这都是一个很好的形式，
		尤其是语句体中含有控制语句，例如return或者break
		if err := file.Chmod(0664);err != nil{
			log.Stderr(err)
			return err
		}
		if true && true {
			fmt.Println("true")
		}
		if !false { //{必须和if在同一行
			fmt.Println("true")
		}
		语句体结束于break continue goto return 不必要的else会被省略
	  */
	  if true {
		fmt.Println("true")
	  }else{
		fmt.Println("false")
	  }
	  if false {
		fmt.Println("true")
	  }else{
		fmt.Println("false")
	  }
	 /**
	  goto 一定是当前函数内定义的标签，标签名大小写敏感
	  for的三种形式
	  for init;condition;post {} //和c语言的相似
	  for condition {}
	  for {} //和c语言的for(;;)一样（死循环）
	  */
	sum := 0
	for i:=0;i<10;i++ {sum+=i}
	fmt.Println("sum:",sum)
	/**
	 go没有逗号表达式，++和-是语句而不是表达式，想执行多个变量，应该使用平行赋值
	 for i,j := 0,len(a)-1;i<j;i,j = i+1,j-1{ a[i],a[j] = a[j], a[i] }
	 */
	for i:=0; i<10;i++ {
		if i > 5{
			break
		}
		fmt.Println(i)
	}
	//嵌套循环式，可以在break后面指定标签，用标签决定那个循环终止
	J:for j:=0;j<5;j++{
		for i:=0;i<10;i++{
			if i > 5{
				break J //跳出最外的循环
			}
			fmt.Println(i)
		}
	}
	/**
	 range 可用于循环，它在slice array string map channel，range是一个迭代器，当被调用时
	 从它循环的内容中返回一个键值对。基于不同的内容，range返回不同的东西
	 当对slice或者array做循环时，range返回序号作为键，这个序号对应的内容作为值
	 */
	list := []string{"a", "b", "c", "d", "e", "f"}
	for k,v := range list {
		fmt.Println("k: ",k," v: ",v) //k:0-5 v:a-f
	}
	for pos,char := range "这是一句汉语" {
		fmt.Printf("pos: %d char: '%c'\n",pos,char) //pos:0 3 6 ... char:这 是 一 句 汉 语
	}
	/**
		switch 表达式不必是常量或者整数，执行过程从上至下，直到找到匹配项
		没有表达式会匹配true
		func unhex(c byte) byte {
			switch {
				case '0' <= c && c<= '9'
					return c - '0'
				case 'a' <= c && c<= 'f'
					return c - 'a' + 10
				case 'A' <= c && c<= 'F'
					return c - 'A' + 10
			}
			return 0
		}
		不会匹配失败后自动向下尝试，但是可以使用fallthrough使其这样做
		switch i {
			case 0: //空的case体
			case 1:
				f()//当i== 0时，f不会被执行
		}
		switch i {
			case 0: fallthrough
			case 1:
				f()//当i== 0时，f会被执行
			default:
				g() //当都不匹配时调用
		}
		switch c {
			case ' ','?','&': f1()//可以使用逗号隔开的列表作为或 
			case '+':
				f()//当i== 0时，f会被执行
			default:
				g() //当都不匹配时调用
		}
	 */
	 s_test1_1 := []byte("abcd")
	 s_test1_2 := []byte("abcd")
	 fmt.Println("test1:",Compare(s_test1_1,s_test1_2)) //0
	 s_test1_1 = []byte("abcd")
	 s_test1_2 = []byte("abce")
	 fmt.Println("test1:",Compare(s_test1_1,s_test1_2))//-1
	 s_test1_1 = []byte("abcd")
	 s_test1_2 = []byte("abcc")
	 fmt.Println("test1:",Compare(s_test1_1,s_test1_2))//1
	 s_test1_1 = []byte("abcd")
	 s_test1_2 = []byte("abc")
	 fmt.Println("test1:",Compare(s_test1_1,s_test1_2))//1
	 s_test1_1 = []byte("abc")
	 s_test1_2 = []byte("abcd")
	 fmt.Println("test1:",Compare(s_test1_1,s_test1_2))//-1
	/**
	 内建函数 预定义了少数函数，无需引用任何包就可以使用它们
		close	new		panic	complex
		delete	make	recover	real
		len		append	print	imag
		cap		copy	println
		close 用于channel通信，使用它来关闭channel
		delete 用于map中删除实例
		len和cap 可用于不同的类型，len用于返回字符串，slice和数组的长度。
		new 用于各种类型的内存分配
		make 用于内建类型 map slice channel的内存分配
		copy 用于复制slice
		append 用于追加slice
		panic和recover 用于异常处理机制，恐慌和恢复
		print println 底层打印函数，可以在不引用fmt包的情况下使用。主要用于调试
		complex real imag 用于复数的处理
	 */
	/**
	 array 在列表中进行多个值的排序，或者使用slice，地点或者哈希类型也可以使用 map
	 array 由 [n]array<type>定义，n表示长度，type表示存储的内容类型
	 a:=[2][2]int{[2]int{1,2},[2]int{3,4}}
	 a:=[2][2]int{{1,2},{3,4}}
	 slice 与array接近，但是在新的元素加入的时候可以增加长度，slice总是指向底层的一个array，
		slice是一个指向array的指针，这是其与array不同的地方，slice是引用类型，这意味着当赋值
		某个slice到另外一个变量，两个引用会指向同一个array。如果一个函数需要一个slice参数，
		在其内对slice元素的修改也会体现在函数调用者中，这和传递底层的array指针类似。
		a := [...]int{1,2,3,4,5}
		s1:=a[2:4]//从序号2到3创建slice 包含元素 3,4
		s2:=a[1:5]//从序号1到4创建slice 包含元素 2,3,4,5
		s3:=a[:]//用array中的所有元素创建slice，是a[0:len(a)]的简化写法
		s4:=a[:4]//从序号0到3创建，这是a[0:4]的简化写法
		s5:=s2[:]//s5仍指向array a
	 */
	 var array [100]int
	 slice :=array[0:99]
	 slice[98] = 'a' //ok
	 //slice[99] = 'a' //error
	/**
	 append向slice s追加零值或其他x值，返回追加后新的，与s有同等类型的slice。
	 如果s没有足够的存储追加的值，append分配一个足够大的新的slice来存放原有slice的元素和追加的值。
	 返回的slice可能指向不同的底层array
	 s0:=[]int{0,0}
	 s1 := append(s0,2) //s1 = 0,0,2
	 s2 := append{s1,3,5,7}//s2 = 0,0,2,3,5,7
	 s3 := append{s2,s0...}//s3 = 0,0,2,3,5,7,0,0
	 */
	 /**
	  copy 从slice源复制到目标，返回复制的元素个数，元和目标可能重叠，复制的数量是len(src)和len(des)中最小值
	  var a = [...]int{0,1,2,3,4,5,6,7}
	  var s = make([]int,6)
	  n1:=copy(s,a[0:])//n1=6 s = 0,1,2,3,4,5
	  n2:=copy(s,s[2:])//n2=4 s = 2,3,4,5,4,5
	  */
	/**
	 map 用字符串做索引的数组
	 monthdays :=map[string]int{
		"Jan":31,"Feb":28,"Mar":31,
		"Apr":30,"May":31,"Jun":30,
		"Jul":31,"Aug":31,"Sep":30,
		"Oct":31,"Nov":30,"Dec":31,
	 }
	 只需要声明一个map的时候，使用make的形式，monthdays:=make(map[string]int)
	 使用，map["Dec"] 即 31
	 year:=0
	 for _,days:=range monthdays {//键没有使用，因此用_,days
		year += days
	 }
	 向map中添加元素，
	 monthdays["Undecim"] = 30 //增加
	 monthdays["Feb"] = 29 //重写元素
	 v,ok:=monthdays["Mar"] //检查元素是否存在
	 delete(monthdays,"Mar")//移除元素
	 */
	 
}
func Compare(a,b []byte) int {
	for i:=0;i < len(a)&&i<len(b);i++{
		switch{
			case a[i] > b[i]:
				return 1
			case a[i] < b[i]:
				return -1
		}
	}
	switch{
		case len(a) < len(b):
			return -1
		case len(a) > len(b):
			return 1
	}
	return 0
}
	 