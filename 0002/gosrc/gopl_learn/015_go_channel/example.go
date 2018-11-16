/**
 * 001 https://blog.csdn.net/whatday/article/details/74453089
 * 002 ch <- v 发送值v到channel ch中
 * 003 v:= <-ch 从channel ch中接收数据，并将数据赋值给v
 * 004 箭头的指向就是数据的流向
 * 005 channel必须先创建再使用
 * 006 ch:=make(chan int)
 * 007 channel类型定义 ChannelType = ("chan"|"chan" "<-"|"<-" "chan") ElementType .
 * 008 包括三种类型的定义，可选的<- 代表了channel的方向，如果没有指定方向那么channel是双向的，既可以接收也可以发送
 * 009 chan T //可以接收和发送类型为T的数据
 * 		chan<- float64 //只可以发送float64类型的数据
 * 		<-chan int //只可以用来接收int类型的数据
 * 010 <- 总是有限和最左边的类型结合
 * 		chan<- chan int 等价于 chan<- (chan int)
 * 		chan<- <-chan int 等价于 chan<- (<-chan int)
 * 		chan (<-chan int)
 * 011 make(chan int,100) 使用make初始化channel,并且可以设置容量
 * 		容量代表channel容纳的最多的元素的数量，代表channel的缓存大小
 * 012 如果没有设置容量，或者容量设置为0, 说明Channel没有缓存，
 *	 	只有sender和receiver都准备好了后它们的通讯(communication)才会
 *	 	发生(Blocking)。如果设置了缓存，就有可能不发生阻塞， 只有buffer满了
 *		 后 send才会阻塞， 而只有缓存空了后receive才会阻塞。一个nil channel不会通信。
 * 013 可以通过内建的close方法关闭channel
 * 014 可以在多个goroutine从/往一个channel中receive/send数据，不必考虑额外的同步措施
 * 015 channel可以作为一个先进先出FIFO队列，接收的数据和发送的数据顺序是一致的
 * 016 channel的receive支持multi-valued assignment
 * 		v,ok := <-ch
 * 017 用来检查channel是否已经关闭
 *		send语句，send语句用来往channel中发送数据，如ch<-3
 *		SendStmt = Channel "<-" Expression .
 *		Channel = Expression .
 * 		在通信开始前channel和expression必选先求值出来，比如下，3+4先计算出7再发送给channel
 * 		c:=make(chan int)
 *		defer close(c)
 *		go func(){c <- 3+4}()
 *		i: = <-c
 *		fmt.Println(i)
 * 018 send被执行前通讯一致被阻塞着，无缓冲的channel只有在receiver准备好后send才被执行，
 *		 如果有缓存，并且缓存未满，send会被执行
 * 019 往一个已经被close的channel中继续发送数据会导致run-time panic
 * 020 往nil channel中发送数据会一直被阻塞着
 * 021 receive操作符
 *		<-ch 用来从channel ch中接收数据，这个表达式会一直被block，直到有数据可以接收
 * 022 从一个nil channel中接收数据会一直被block
 * 023 从一个呗close的channel中接收数据不会被阻塞，而是立即返回，接收完已发送的
 *		数据后会返回元素类型的零值
 * 024 可以使用一个额外的参数检查channel是否关闭
 *		x,ok := <-ch
 *		x,ok = <-ch
 *		var x,ok = <-ch
 *		如果ok是false，表明接收的x是产生的零值，这个channel被关闭了或者为空
 * 025 blocking
 *		缺省情况下，发送和接收会一直阻塞着，直到另一方准备好。
 *		这种方式可以用来在gororutine中进行同步，而不必使用显示的锁或者条件变量。
 * 026 test_01()
 * 027 buffered channels
 *		make 的第二个参数指定缓存的大小，ch:= make(chan int,100)
 *		通过缓存的使用，尽可能的避免阻塞，提高应用的性能
 * 028 test_02() for range语句处理channel
 * 029 select 语句选择一组可能的send操作和receive操作去处理，类似switch，但是
 *		只是用来处理通讯操作，它的case可以是send语句，也可以是receive语句或者default
 * 		receive语句可以将值赋值给一个或者两个变量，它必须是一个receive操作
 *		最多允许有一个default case 它可以放在case列表的任何位置，一般放在最后
 *		test_03()
 *		 如果有同时多个case去处理,比如同时有多个channel可以接收数据，那么Go会伪随机
 *		的选择一个case处理(pseudo-random)。如果没有case需要处理，则会选择default去
 *		处理，如果default case存在的情况下。如果没有default case，则select语句会阻塞，
 *		直到某个case需要处理。
 *		需要注意的是，nil channel上的操作会一直被阻塞，如果没有default case,只有nil channel
 *		的select会一直被阻塞。
 *		select语句和switch语句一样，它不是循环，它只会选择一个case来处理，如果想一直处理channel，
 *		你可以在外面加一个无限的for循环：
 * 030 select的超时操作 timeout
 *		select有很重要的一个应用就是超时处理。 因为上面提到，如果没有case需要处理，select语句
 *		就会一直阻塞着。这时候可能就需要一个超时操作，用来处理超时的情况。
 *		test_04例子会在2秒后往channel c1中发送一个数据，但是select设置为1秒超时,因此会打印
 *		出timeout 1,而不是result 1。
 * 031 其实它利用的是time.After方法，它返回一个类型为<-chan Time的单向的channel，在指定的时
 *		间发送一个当前时间给返回的channel中。
 * 032 Timer和Ticker
 *		关于时间的两个Channel。
 *			timer是一个定时器，代表未来的一个单一事件，你可以告诉timer你要等待多长时间，它提供
 *			一个Channel，在将来的那个时间那个Channel提供了一个时间值。
 *			下面的例子中第二行会阻塞2秒钟左右的时间，直到时间到了才会继续执行。
 *			timer1 := time.NewTimer(time.Second * 2)
 *			<-timer1.C
 *			fmt.Println("Timer 1 expired")
 *			只是想单纯的等待的话，可以使用time.Sleep来实现 还可以使用timer.Stop来停止计时器
 *			timer2 := time.NewTimer(time.Second)
 *			go func() {
 *				<-timer2.C
 *				fmt.Println("Timer 2 expired")
 *			}()
 *			stop2 := timer2.Stop()
 *			if stop2 {
 *				fmt.Println("Timer 2 stopped")
 *			}
 *		类似timer, ticker也可以通过Stop方法来停止。一旦它停止，接收者不再会从channel中接收数据了。
 * 033 close
 *		内建的close方法可以用来关闭channel
 *		channel关闭后sender的receiver操作
 *		channel c已经被关闭,继续往它发送数据会导致panic: send on closed channel:
 *		go func() {
 *			time.Sleep(time.Hour)
 *		}()
 *		c := make(chan int, 10)
 *		c <- 1
 *		c <- 2
 *		close(c)
 *		c <- 3
 *		但是从这个关闭的channel中不但可以读取出已发送的数据，还可以不断的读取零值:
 *		c := make(chan int, 10)
 *		c <- 1
 *		c <- 2
 *		close(c)
 *	    fmt.Println(<-c) //1
 *	    fmt.Println(<-c) //2
 *	    fmt.Println(<-c) //0
 *	    fmt.Println(<-c) //0
 *		但是如果通过range读取，channel关闭后for循环会跳出：
 *		c := make(chan int, 10)
 *		c <- 1
 *		c <- 2
 *		close(c)
 *		for i := range c {
 *			fmt.Println(i)
 *		}
 *		通过i, ok := <-c可以查看Channel的状态，判断值是零值还是正常读取的值。
 *		c := make(chan int, 10)
 *		close(c)
 *		i, ok := <-c
 *		fmt.Printf("%d, %t", i, ok) //0, false
 * 034 同步channel可以用在goroutine之间的同步
 *		test_05中 goroutine通过done channel等待worker完成任务。 worker做完任务后只
 *		需往channel发送一个数据就可以通知main goroutine任务完成
 */
package main
import (
	"fmt"
	"time"
)
func main(){
	test_01()
	test_02()
	test_03()
	test_04()
	test_05()
}
func test_01(){
	s:= []int{7,2,8,-9,4,0}
	c:=make(chan int)
	go sum(s[:len(s)/2],c)
	go sum(s[len(s)/2:],c)
	x,y := <-c,<-c //receive from c
	fmt.Println(x,"+",y,"=",x+y)//x = -5 y = 17 x+y = 12
}
func sum(s []int,c chan int){
	sum:=0
	for _,v := range s{
		sum += v
	}
	c<- sum //send sum to c
}
func test_02(){
	go func(){
		time.Sleep(1*time.Hour)
	}()
	c:=make(chan int)
	go func(){
		for i := 0;i<10;i=i+1{
			c <- i//向c中写数据
			fmt.Println("write:",i)
		}
		/*
		 * range c产生的迭代值为Channel中发送的值，它会一直迭代直到channel被关闭。
		 * 上面的例子中如果把close(c)注释掉，程序会一直阻塞在for …… range那一行。
		 */
		close(c)
	}()
	for i:=range c{//从c中读数据
		fmt.Println(i)
		fmt.Println("read:",i)
	}
	fmt.Println("Finished")
}
func test_03(){
	c:=make(chan int)
	quit := make(chan int)
	go func(){
		for i:=0;i<10;i++{
			fmt.Println(<-c)//从c读取数据打印 flag1
		}
		quit<- 0//将0写入到quit中
	}()
	fmt.Println("before fibonacci")
	fibonacci(c,quit)
	fmt.Println("after fibonacci")
}
func fibonacci(c,quit chan int){
	x,y := 0,1
	for{
		select{
			case c <- x: //将x数据写入到c中,由flag1读取
				fmt.Println("x+y")
				x,y = y,x+y
			case <-quit://结束时才会走到quit有数据
				fmt.Println("quit")
				return
		}
	}
}
func test_04(){
	c1:=make(chan string,1)
	go func(){
		time.Sleep(time.Second * 2)//延迟两秒发送,没有这行的话  打印result 1 有这行 打印timeout 1
		c1 <- "result 1"
	}()
	select{
		case res := <-c1:
			fmt.Println(res)
		case <- time.After(time.Second * 1)://一秒后未接收到数据
			fmt.Println("timeout 1")
	}
}
func worker(done chan bool){
	time.Sleep(time.Second)
	//通知任务已完成
	done <- true
}
func test_05(){
	done := make(chan bool,1)
	go worker(done)
	//等待任务完成
	<-done
}