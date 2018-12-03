package list
import (
	"fmt"
)
// 数据接口
type ElemType interface{}
// 节点
type Node struct{
	Data ElemType
	Pre *Node
	Next *Node
}
// 链表
//type List struct{
//	First *Node
//	Last *Node
//}
// 初始化链表
func InitList() *Node{
	s := new(Node)
	s.Next,s.Pre = s,s
	s.Data = nil
	return &Node{nil,s,s}
}
func add(s,after,before *Node){
	after.Next = s
	s.Pre = after

	s.Next = before
	before.Pre = s
}
func (list *Node)Add_tail(x ElemType){
	s := new(Node)
	s.Data = x
	add(s,list.Pre,list)
}
func (list *Node)IsEmpty() bool {
	//fmt.Println("list.Pre:",list.Pre,"list:", list)
	//panic(nil)
	if list.Pre.Next == list.Next && list.Pre.Pre == list.Pre { // 直接用list.Pre == list 好像不对，list的值为0 list.pre 数据为nil
	//if list.Pre == list {
		//fmt.Println("true")
		return true
	}
	//fmt.Println("false")
	return false
}
func (list *Node) Print() error {
	defer fmt.Println("Print end")
	if list.IsEmpty() {
		fmt.Println("this list is am empty list")
		return nil
	}
	//panic(nil)
	s := list.Next.Next //讲道理的话应该是list.Next
	for s != list {
		fmt.Println("Data is :",s.Data)
		s = s.Next
	}
	
	return nil
}