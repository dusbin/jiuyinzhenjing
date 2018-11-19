package main

import (
	"bufio"
	"os"
	"fmt"
	"./pipeline"
)
func main(){
	const filename = "large.in"
	const n = 100000000
	file,err:=os.Create(filename)
	if err != nil{
		panic(err)//抛出异常
	}
	defer file.Close() //退出时进行执行，即使中间退出也会执行
	p:=pipeline.RandomSource(n)//生成50个随机数
	pipeline.WriterSink(bufio.NewWriter(file),p)//写数据 使用bufio 提高效率
	file,err = os.Open(filename)
	if err != nil{
		panic(err)
	}
	defer file.Close()//退出时先执行，再执行上一个defer,遵循先进后出原则
	p = pipeline.ReaderSource(bufio.NewReader(file))//读数据
	count :=0 
	for v:= range p{
		fmt.Println(v)
		count++
		if count > 100{
			break
		}
	}
}
func merageDemo(){
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
