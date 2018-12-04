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
	s.Pre,s.Next = nil,nil
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
func list_splice(add,after,before *Node){
	first,last := add.Next,add.Pre
	first.Pre = after
	after.Next = first
	last.Next = before
	before.Pre = last
}
// 将链表add尾插到list链表上
func (list *Node)List_splice_tail(add *Node)bool {
	if list == add {
		return false
	}
	if !add.IsEmpty(){
		list_splice(add,list.Pre,list)
		InitList(add)
	}
	return true
}
// 将链表add头插到list链表上
func (list *Node)List_splice_head(add *Node)bool {
	if list == add {
		return false
	}
	if !add.IsEmpty(){
		list_splice(add,list,list.Next)
		InitList(add)
	}
	return true
}
// 对列表中的所有元素均执行 参数为函数
func (list *Node)Foreach(f func(*Node)) bool {
	if list.IsEmpty(){
		return false
	}
	s := list.Next
	for s != list{
		f(s)
		s = s.Next
	}
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