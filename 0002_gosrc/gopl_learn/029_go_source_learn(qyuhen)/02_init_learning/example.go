package main
import (
	"fmt"
	_ "net/http" // 引入一个标准库里的包 但是不使用
)
func init(){
	fmt.Println("main.init.2")
}
func main(){
	test()
}
func init(){
	fmt.Println("main.init.1")
}