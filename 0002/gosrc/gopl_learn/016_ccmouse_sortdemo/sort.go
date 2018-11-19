package main

import (
	"fmt"
	"sort"
	"./pipeline"
)
func main(){
	//slice 比数组更灵活  slice of int 
	a:=[]int{3,6,2,1,9,10,8}
	sort.Ints(a)
	for _,v := range a{
		fmt.Println(v)
	}
	p:=pipeline.ArraySource(3,2,6,7,4)
	for{
		if num,ok:=<-p;ok{//从goroutine中读取数据，当ok为真时，获取到的是channel中的数据，当ok为假，获取到int的零值
			fmt.Println(num)
		}else{
			break
		}
	}

}
