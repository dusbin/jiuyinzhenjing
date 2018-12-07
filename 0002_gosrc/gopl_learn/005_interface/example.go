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
func u_set(r Retriever,url string){
	r.Set(url)
}
func u_get(r Retriever) string {
	return r.Get()
}
func u_add(r Retriever,url string) string {
	return r.Add(url)
}
//第一种使用方法
func test_01(){
	var r Retriever
	r = &retiver.Retriever{""} // 具体接口，并且进行初始化
	u_set(r,"192.168.1.71") // 具体由retiver包实现
	fmt.Println(u_get(r)) // 具体由retiver包实现
	fmt.Println(u_add(r,":80")) // 具体由retiver包实现
}
// 第二种方法
func test_02(){
	// 以下三个retiver.Retriever{} 并不是一个  
	// 输出为 "" ":80",也就是说u_set的结果u_get不到，u_get的结果u_add不了，所以不推荐这个方法
	u_set(&retiver.Retriever{""},"192.168.1.72") //直接传入接口
	fmt.Println(u_get(&retiver.Retriever{}))
	fmt.Println(u_add(&retiver.Retriever{},":80"))
}
func main(){
	test_01()
	test_02()
}