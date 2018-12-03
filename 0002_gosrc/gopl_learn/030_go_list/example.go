package main
import (
	"./doubleLinkedList"
	"./list"
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
func test_03(){
	list_i := new(list.Node)
	list.InitList(list_i)
	val := []int{1,2,3,4,5}
	list_i.Print()
	for _,v := range val {
		list_i.Add_tail(v)
	}
	list_i.Print()
}
func test_04(){
	list_s := new(list.Node)
	list.InitList(list_s)
	val := []string{"wo","shi","cheng","xu","yuan"}
	list_s.Print()
	for _,v := range val {
		list_s.Add_tail(v)
	}
	list_s.Print()
}

func test_05(){
	list_s := new(list.Node)
	list.InitList(list_s)
	val := []string{"wo","shi","cheng","xu","yuan"}
	list_s.Print()
	for _,v := range val {
		list_s.Add_head(v)
	}
	list_s.Print()
	list_s.DeleteVal("shi")
	list_s.Print()
	list_s.DeleteVal("xu")
	list_s.Print()
}

func main(){
	//test_01()
	//test_02()
	test_03()
	test_04()
	test_05()
}