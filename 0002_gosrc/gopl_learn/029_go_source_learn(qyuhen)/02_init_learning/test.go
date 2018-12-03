package main
import (
	"fmt"
	"./lib"
)
// init函数
func init(){
	fmt.Println("test.init")
}
// test函数
func test(){
	fmt.Println(lib.Sum(1,2,3))
}
