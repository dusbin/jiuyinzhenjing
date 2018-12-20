package main
import (
	"time"
	"fmt"
)
// https://studygolang.com/articles/17102
func workerPool(n int,jobCh <-chan int,retCh chan<- string){
	for i := 0;i < n;i++{
		go worker(i,jobCh,retCh)
	}
}
func worker(id int,jobCh <-chan int,retCh chan<- string){
	for job := range jobCh{
		ret := fmt.Sprintf("Worker %d processed job: %d",id,job)
		retCh <- ret
	}
}
func main(){
	jobCh := genJob(100)
	retCh := make(chan string,200)
	workerPool(5,jobCh,retCh)
	time.Sleep(time.Second)
	close(retCh)
	for ret := range retCh{
		fmt.Println(ret)
	}
}
func genJob(n int) <-chan int {
	jobCh:= make(chan int,200)
	go func() {
		for i := 0; i < n; i++{
			jobCh <- i
		}
		close(jobCh)
	}()
	return jobCh
}