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
// 在after和before之间添加s
func add(s,after,before *Node){
	after.Next = s
	s.Pre = after
	s.Next = before
	before.Pre = s
}
// 将x元素添加到链表尾部
func (list *Node)Add_tail(x ElemType){
	s := new(Node)
	s.Data = x
	add(s,list.Pre,list)
}
// 将x元素添加到链表头部
func (list *Node)Add_head(x ElemType){
	s := new(Node)
	s.Data = x
	add(s,list,list.Next)
}
// 判断链表是否为空
func (list *Node)IsEmpty() bool {
	if list.Pre == list {
		return true
	}
	return false
}
// 从头到尾查找元素
func (list *Node)Find(x ElemType) *Node{
	if list.IsEmpty() {
		return nil
	}
	s := list.Next
	for s != list{
		if x == s.Data{
			return s
		} else {
			s = s.Next
		}
	}
	return nil
}
func delete(pre,next *Node){
	pre.Next = next
	next.Pre = pre
}
// 删除节点
func (list *Node)Delete(s *Node){
	delete(s.Pre,s.Next)
}
// 删除从头到尾找到的第一个元素
func (list *Node)DeleteVal(x ElemType) bool {
	result := list.Find(x)
	if result == nil {
		return false
	}
	list.Delete(result)
	return true
}
func (list *Node)MoveToHead(s *Node){
	list.Delete(s)
	add(s,list,list.Next)
}
// 将元素移到队首
func (list *Node)MoveValToHead(x ElemType) bool {
	result := list.Find(x)
	if result == nil {
		return false
	}
	list.MoveToHead(result)
	return true
}
func (list *Node)MoveToTail(s *Node){
	list.Delete(s)
	add(s,list.Pre,list)
}
// 将元素移到队尾
func (list *Node)MoveValToTail(x ElemType) bool {
	result := list.Find(x)
	if result == nil {
		return false
	}
	list.MoveToTail(result)
	return true
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