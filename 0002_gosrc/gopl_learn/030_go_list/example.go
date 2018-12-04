package main
import (
	"fmt"
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
	list_s.MoveValToHead("shi")
	list_s.Print()
	list_s.MoveValToTail("xu")
	list_s.Print()
	list_s.DeleteVal("shi")
	list_s.Print()
	list_s.DeleteVal("xu")
	list_s.Print()
}
func test_06(){
	list_s := new(list.Node)
	list.InitList(list_s)
	list_s2 := new(list.Node)
	list.InitList(list_s2)
	val := []string{"wo","shi","cheng","xu","yuan"}
	list_s.Print()
	list_s2.Print()
	for _,v := range val {
		list_s.Add_head(v)
		list_s2.Add_tail(v)
	}
	list_s.Print()
	list_s2.Print()
	ok := list_s.List_splice_head(list_s)
	if ok {
		fmt.Println("success")
		list_s.Print()
		list_s2.Print()
	}else{
		fmt.Println("fail")
	}
	ok = list_s.List_splice_head(list_s2)
	if ok {
		fmt.Println("success")
		list_s.Print()
		list_s2.Print()
	}else{
		fmt.Println("fail")
	}
}
func test_07(){
	list_s := new(list.Node)
	list.InitList(list_s)
	list_s2 := new(list.Node)
	list.InitList(list_s2)
	val := []string{"wo","shi","cheng","xu","yuan"}
	list_s.Print()
	list_s2.Print()
	for _,v := range val {
		list_s.Add_head(v)
		list_s2.Add_tail(v)
	}
	list_s.Print()
	list_s2.Print()
	ok := list_s.List_splice_tail(list_s)
	if ok {
		fmt.Println("success")
		list_s.Print()
		list_s2.Print()
	}else{
		fmt.Println("fail")
	}
	ok = list_s.List_splice_tail(list_s2)
	if ok {
		fmt.Println("success")
		list_s.Print()
		list_s2.Print()
	}else{
		fmt.Println("fail")
	}
}
func printNode(s *list.Node){
	fmt.Println("Data:",s.Data)
}
func changeData(s *list.Node){
	s.Data = "123"
}
func test_08(){
	list_s := new(list.Node)
	list.InitList(list_s)
	val := []string{"wo","shi","cheng","xu","yuan"}
	list_s.Print()
	for _,v := range val {
		list_s.Add_head(v)
	}
	list_s.Foreach(printNode)
	list_s.Foreach(changeData)
	list_s.Foreach(printNode)
}
func main(){
	//test_01()
	//test_02()
	//test_03()
	//test_04()
	//test_05()
	//test_06()
	//test_07()
	test_08()
}