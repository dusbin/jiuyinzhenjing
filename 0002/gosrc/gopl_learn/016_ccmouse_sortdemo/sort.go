package main

import (
	"bufio"
	"os"
	"fmt"
	"./pipeline"
)
func main(){
	p := createPipeline("small.in",512,4)
	writeToFile(p,"small.out")
	printFile("small.out")
	//merageDemo_2()
}
func printFile(filename string){
	file,err:= os.Open(filename)
	if err != nil{
		panic(err)
	}
	defer file.Close()
	p:=pipeline.ReaderSource(file,-1)
	for v:= range p{
		fmt.Println(v)
	}

}
//写入文件
func writeToFile(p <- chan int,filename string){
	file,err:=os.Create(filename)
	if err != nil{
		panic(err)
	}
	defer file.Close()
	writer:=bufio.NewWriter(file)
	defer writer.Flush()//先于上两行的defer执行，先进后出
	pipeline.WriterSink(writer,p) //将p写入到文件中去
}
//创建pipeline 文件名 文件大小，chunk个数
func createPipeline(filename string,fileSize,chunkCount int)<-chan int{//没有实现工业化的Fileclose
	chunckSize := fileSize / chunkCount //每一块的大小 产品中需要处理非整除的情况
	sortResults:=[]<-chan int{} //用来接收pipeline.InMemSort的结果
	for i:=0;i<chunkCount;i++{
		file,err:=os.Open(filename)
		if err!=nil{
			panic(err)
		}
		file.Seek(int64(i*chunckSize),0)
		source:=pipeline.ReaderSource(bufio.NewReader(file),chunckSize) //读到数据送到source
		sortResults = append(sortResults, pipeline.InMemSort(source))//将结果加入到sortResults中
	}
	return pipeline.MergeN(sortResults...)//将归并后的数结果返回回去

}
func merageDemo_2(){
	const filename = "small.in"
	const n = 64
	file,err:=os.Create(filename)
	if err != nil{
		panic(err)//抛出异常
	}
	defer file.Close() //退出时进行执行，即使中间退出也会执行
	p:=pipeline.RandomSource(n)//生成50个随机数
	writer := bufio.NewWriter(file)//创建buf提高效率
	pipeline.WriterSink(writer,p)//写数据
	writer.Flush() //没有这一行的话可能生成的大小不是想要的，没有把数据全部写入到文件中
	file,err = os.Open(filename)
	if err != nil{
		panic(err)
	}
	defer file.Close()//退出时先执行，再执行上一个defer,遵循先进后出原则
	p = pipeline.ReaderSource(bufio.NewReader(file),-1)//读数据
	count :=0 
	for v:= range p{
		fmt.Println(v)
		count++
		if count > 100{
			break
		}
	}
}
func merageDemo_1(){
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
