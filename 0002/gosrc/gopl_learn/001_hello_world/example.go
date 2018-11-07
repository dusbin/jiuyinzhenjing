package main
import(
	"fmt"
	"unicode/utf8"
)
func main(){
	test1()
	test2()
	test3()
	test4()
	test5()
	test6()
}
func test1(){
	fmt.Println("test1")
	for i := 0;i<10;i++{
		fmt.Println(i)
	}
}
func test2(){
	fmt.Println("test2")
	i := 0
BEGIN:
	if i < 10 {
		fmt.Println(i)
		i++
		goto BEGIN
	}
}
func test3(){
	fmt.Println("test3")
	for i := 1;i<101;i++{
		if i%3 == 0 && i%5 == 0 {
			fmt.Println("FizzBuzz")
			continue
		}
		if i%3 == 0 {
			fmt.Println("Fizz")
			continue
		}
		if i%5 == 0 {
			fmt.Println("Buzz")
			continue
		}
		fmt.Println(i)
	}
}
func test4(){
	fmt.Println("test4")
	s := "A"
	for i := 0;i<10;i++{
		fmt.Println(s)
		s = s+"A"
	}
}
func test5(){
	str := "dsjkdshdsdc...js,"
	str1:= "这是一句汉语"
	fmt.Println("String:",str,",Length:",len([]byte(str)),",Runes:",utf8.RuneCount([]byte(str)))
	fmt.Println("String:",str1,",Length:",len([]byte(str1)),",Runes:",utf8.RuneCount([]byte(str1)))
}
func test6(){
	s:= "foobar"
	a:=[]byte(s)
	for i,j := 0,len(a)-1;i<j;i,j = i+1,j-1 {
		a[i],a[j] = a[j],a[i]
	}
	fmt.Println(string(a))
	s= "这是一句汉语"
	a=[]byte(s)
	for i,j := 0,utf8.RuneCount(a)-1;i<j;i,j = i+1,j-1 {
		a[i],a[j] = a[j],a[i]
	}
	fmt.Println(string(a))//汉语宽字符存在问题
}
