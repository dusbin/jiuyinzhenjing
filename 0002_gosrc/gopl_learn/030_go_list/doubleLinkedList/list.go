package doubleLinkedList
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
type List struct{
	First *Node
	Last *Node
	Size int
}
// 工厂函数
func CreateList() *List{
	s := new(Node)
	s.Next,s.Pre = s,s
	return &List{s,s,0}
}
// 尾插法
func (list *List)PushBack(x ElemType){
	s := new(Node)
	s.Data = x
	list.Last.Next = s
	s.Pre = list.Last

	list.Last = s
	list.Last.Next = list.First
	list.First.Pre = list.Last
	list.Size++
}
// 头插法
func (list *List)PushFront(x ElemType){
	s := new(Node)
	s.Data = x
	s.Next = list.First.Next
	list.First.Next.Pre = s

	list.First.Next = s
	if list.Size == 0 {
		list.Last = s
	}
	list.Size++
}
// 尾删法
func (list *List) PopBack() bool {
	if list.IsEmpty(){
		return false
	}
	s := list.Last.Pre // 找到最后一个节点的前驱
	s.Next = list.First
	list.Last = s
	list.Size--
	return true
}
// 头删法
func (list *List)PopFront() bool {
	if list.IsEmpty(){
		return false
	}
	s := list.First.Next //找到第一个节点
	list.First.Next = s.Next
	s.Next.Pre = list.First
	if list.Size == 1 {
		list.Last = list.First
	}
	list.Size--
	return true
}
// 查找指定元素
func (list *List)Find(x ElemType) *Node{
	s := list.First.Next
	for s != list.First{
		if x == s.Data{
			return s
		} else {
			s = s.Next
		}
	}
	return nil
}
// 按值删除节点
func (list *List)DeleteVal(x ElemType) bool {
	s := list.Find(x)
	if s != nil {
		s.Pre.Next = s.Next
		s.Next.Pre = s.Pre
		list.Size--
		// 如果删除的是最后一个节点
		if s == list.Last {
			list.Last = s.Pre
		}
		return true
	}
	return false
}
// 把值为x的元素的值修改为y
func (list *List)Modify(x,y ElemType) bool {
	s := list.Find(x)
	if s != nil{
		s.Data = y
		return true
	}
	return true
}
// 判断链表是否为空
func (list *List)IsEmpty() bool {
	return list.Size == 0
}
// 反转链表
// 保留第一个节点，将剩余的节点游离出来，然后依次头插到保留的节点中
func (list *List)Reverse(){
	if list.Size > 1 {
		s := list.First.Next
		p := s.Next
		s.Next = list.First // 第一个节点逆置后成为最后一个节点
		list.Last = s

		for p != list.First {
			s = p
			p = p.Next

			s.Next = list.First.Next
			list.First.Next.Pre = s

			s.Pre = list.First
			list.First.Next = s
		}
	}
}
func (list *List) Print() error {
	if list.IsEmpty() {
		fmt.Println("this list is am empty list")
		return nil
	}
	s := list.First.Next
	for s != list.First {
		fmt.Println("Data is :",s.Data)
		s = s.Next
	}
	fmt.Println()
	return nil
}