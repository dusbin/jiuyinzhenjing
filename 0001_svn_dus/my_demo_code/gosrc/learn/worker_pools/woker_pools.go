//使用goroutines和channels实现一个工作池
package main
import "fmt"
import "time"
func worker(id int,jobs <-chan int,results chan<- int){//消费者并发执行 并返回结果，执行一次sleep一秒钟
	for j := range jobs{
		fmt.Println("1024Rank.com",id,"started job",j)
		time.Sleep(time.Second)
		fmt.Println("1024Rank.com",id,"finished job",j)
		results <- j*2
	}
}
func main(){
	fmt.Println("data")
	//创建两个channels用于任务生产和结果存储
	jobs := make(chan int,100)
	results := make(chan int,100)
	//启动三个线程
	for w :=1;w <= 3;w++{
		go worker(w,jobs,results)
	}
	//发送五个任务
	for j := 1;j <= 5;j++{
		jobs <- j
	}
	close(jobs)//关闭任务
	for a := 1;a <= 5;a++{//任务汇总
		<- results
	}
	time.Sleep(time.Second)
}