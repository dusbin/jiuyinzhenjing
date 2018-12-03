package main
import (
	"./doubleLinkedList"
)
func test_01(){
	list := doubleLinkedList.CreateList()
	s := []int{1,2,3,4,5,6,7}
	for _,v := range s {
		list.PushBack(v)
	}
	list.Print()
	list.Modify(5,55)
	list.Print()
	list.DeleteVal(7)
	list.Print()
	list.PopBack()
	list.Print()
	list.PopFront()
	list.Print()
	list.Reverse()
	list.Print()
}
func test_02(){
	list := doubleLinkedList.CreateList()
	s := []string{"Asia","China","Shaanxi","Xianyang","weicheng","beidu"}
	for _,v := range s {
		list.PushBack(v)
	}
	list.Print()
	list.Modify("Xianyang","Xi'an")
	list.Print()
	list.DeleteVal("beidu")
	list.Print()
	list.PopBack()
	list.Print()
	list.PopFront()
	list.Print()
	list.Reverse()
	list.Print()
}
func main(){
	test_01()
	test_02()
}