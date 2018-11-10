package main
import(
	"fmt"
)
func main(){
	a,b := 2,7
	a,b = test_2(a,b)
	fmt.Println(a," ",b)
	a,b = 4,3
	a,b = test_2(a,b)
	fmt.Println(a," ",b)
	input:=[]float64{1.5,2.4,3.7,8.5}
	fmt.Println("avge:",test_1(input))
	test_3()
	test_4()
	test_5()
	test_6()
	test_7()
	test_8()
	test_9()
}
//函数返回一个函数
func test_9(){
	p1:=plusTwo()
	fmt.Println(p1(2))//4
	fmt.Println(p1(3))//5
	p2 := plusX(3)
	fmt.Println(p2(4))//7
}
func plusTwo() func(int) int { //定义新的函数返回一个函数
	return func(x int) int {return x +2} //函数符号，返回语句中定义的+2
}
func plusX(x int) func(int) int{ //定义新的函数返回一个函数
	return func(y int) int {return x +y} //函数中定义使用局部变量x
}
//冒泡排序
func test_8(){
	input:= []int{3,5,4,7,6,11,10,13,12}
	fmt.Println(input)
	bubblesort(input)
	fmt.Println(input)
}
func bubblesort(input []int){
	for i:=0;i<len(input)-1;i++{
		for j:=i+1;j<len(input);j++{
			if input[j] <input[i]{
				input[i],input[j] = input[j],input[i]
			}
		}
	}
	return
}
//查看数组最大值
func test_7(){
	input:= []int{3,5,4,7,6,11,10,13,12}
	fmt.Println(max(input))//13
	
}
func max(input []int)(x int){
	x = 0
	for _,tmp:= range input{
		if tmp > x{
			x = tmp
		}
	}
	return
}
//斐波拉契
func test_6(){
	fmt.Println("fibonacci")
	fmt.Println(fibonacci(5))
	fmt.Println(fibonacci(6))
	fmt.Println(fibonacci(7))
}
func fibonacci(value int) []int{
	x:=make([]int,value)
	x[0],x[1] =1,1
	for n:=2;n<value;n++{
		x[n] = x[n-1]+x[n-2]
	}
	return x
}
//变参
func test_5(){
	printItem(1,2,3)
	printItem(1,2,3,4)
	printItem(0,1,2,3,4,5)
}
func printItem(num ... int){
	fmt.Println("------printItem--------")
	for _,i:=range num{
		fmt.Println(i)
	}
	return
}
//Q6 平均值
func test_1(input []float64)(output float64){
	sum:=0.0
	switch len(input){
		case 0:
			output = 0
		default:
			for _,tmp:= range input{
				sum = sum+tmp
			}
			output = sum/float64(len(input))
	}
	return
}
//Q7 整数排序
func test_2(a int,b int)(c int,d int){
	if a > b {
		c,d = b,a
	}else{
		c,d = a,b
	}
	return
}
//Q8作用域
func test_3(){
	for i:=0;i<10;i++{
		fmt.Println(i)
	}
	//fmt.Println(i)//i的作用域为for循环,提示i未定义
	/**
	如下也是可以的 i的作用域不仅限于for循环 := 是定义既赋值
	i:=0
	for i=0;i<10;i++{
		fmt.Println(i)
	}
	fmt.Println(i)
	*/
}
//Q9 实现栈
func test_4(){
	var s stack
	s.push(25) //25
	fmt.Println(s)
	s.push(14) //25 14
	fmt.Println(s)
	s.push(13) //25 14 13
	fmt.Println(s)
	fmt.Println(s.pop()) //25 14
	s.push(12) //25 14 12
	fmt.Println(s)
	fmt.Println(s.pop()) //12
	fmt.Println(s.pop()) //14
	fmt.Println(s.pop()) //25
}
type stack struct{
	i int //当前有多少个元素
	data [10]int

}//结构体
func (s *stack) push(k int){
	if s.i+1 > 9{
		return
	}
	s.data[s.i] = k
	s.i++
	return
}
func (s *stack)pop() int{
	s.i--
	return s.data[s.i]
}
