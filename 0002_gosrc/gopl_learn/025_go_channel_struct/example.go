package main

import (
	"time"
	"fmt"
)
func main(){
	//test_01()
	test_02()
}
//等待work的结束
func test_02(){
	done := make(chan struct{})
	go func(){
		work(5)
		close(done)
	}()
	<-done
}
func work(v int){
	for i := 0;i < v;i++{
		time.Sleep(time.Second)
		fmt.Println("wait")
	}
}
//创建并且解除
func test_01(){
	var sig = make(chan struct{})
	close(sig) //必须使用sig进行close，才能执行<-sig,否则会死锁
	<-sig
}