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
// 初始化链表
func InitList(s *Node)(){
	//s := new(Node)
	s.Next,s.Pre = s,s
	s.Data = nil
	//return &Node{nil,s,s} //相当于用s重新构造了一个对象，新的对象的地址不是s，不能和s构成一个环
	return 
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
func (list *Node)Add_head(x ElemType){
	s := new(Node)
	s.Data = x
	add(s,list,list.Next)
}
func (list *Node)IsEmpty() bool {
	if list.Pre == list {
		return true
	}
	return false
}
func (list *Node) Print() error {
	fmt.Println("++++Print begin++++")
	defer fmt.Println("-----Print end-----")
	if list.IsEmpty() {
		fmt.Println("list is empty")
		return nil
	}
	s := list.Next
	for s != list {
		fmt.Println("Data is :",s.Data)
		s = s.Next
	}
	return nil
}