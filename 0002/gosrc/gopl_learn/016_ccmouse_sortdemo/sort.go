package main

import (
	"fmt"
	"./pipeline"
)
func main(){
	//slice 比数组更灵活  slice of int 
	//a:=[]int{3,6,2,1,9,10,8}
	//sort.Ints(a)
	//for _,v := range a{
	//	fmt.Println(v)
	//}
	p:= pipeline.Merge(//归并
			pipeline.InMemSort(//第一个有序数据
				pipeline.ArraySource(3,2,6,7,4)),//将无序数据进行输入排序
			pipeline.InMemSort(//第二个有序数据
				pipeline.ArraySource(7,4,0,3,2,13,8)))//将pipeline.ArraySource的输出作为pipeline.InMemSort的输入
	/*
	for{
		if num,ok:=<-p;ok{//从goroutine中读取数据，当ok为真时，获取到的是channel中的数据，当ok为假，获取到int的零值
			fmt.Println(num)
		}else{
			break
		}
	}
	*/
	for v:=range p{//一定要close，不close的话for一直在读取
		fmt.Println(v)
	}

}
