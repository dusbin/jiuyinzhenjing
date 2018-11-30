package main
import(
	"fmt"
	"time"
)
var i int
func ready(w string,sec int){
	i++
	time.Sleep(time.Duration(sec)*time.Second)
	fmt.Println(w,"is ready!")
	i--
}
func test_1(){
	i = 0
	go ready("Tea",2)//两秒后
	go ready("Coffee",1)//一秒后
	go ready("Co",5)
	fmt.Println("I'm waiting")//立刻打印
BEGIN:
	if i > 0 {
		time.Sleep(1 * time.Second)
		fmt.Println("waiting ready end i(",i,")")
		goto BEGIN
	}
	//time.Sleep(5 * time.Second)//不等待的话程序就退出了，等不到ready函数执行就退出了
}
func main(){
	test_1()
}