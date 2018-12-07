package main
import (
	"fmt"
	"./retiver"
)
// 由调用者定义接口。
type Retriever interface{
	Get()string // 不需要func，默认类似于虚拟函数
	Set(url string) //
	Add(url string) string
}
func u_set(r Retriever){
	r.Set("192.168.40.75")
}
func u_get(r Retriever) string {
	return r.Get()
}
func u_add(r Retriever) string {
	return r.Add(":80")
}
func main(){
	var r Retriever
	r = &retiver.Retriever{""} // 具体接口，并且进行初始化
	u_set(r) // 具体由retiver包实现
	fmt.Println(u_get(r)) // 具体由retiver包实现
	fmt.Println(u_add(r)) // 具体由retiver包实现
}