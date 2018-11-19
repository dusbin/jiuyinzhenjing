package pipeline
func ArraySource(a...int/*a...int 是一个可变参数*/)chan int/*chan int 返回一个int的channel*/{
	out:=make(chan int)
	//channel 是goroutine和goroutine之间的通信
	go func(){
		for _,v := range a{
			out<- v //将a中的元素一个一个写入到channel out中去
		}
		close(out)//发送完后关闭掉channel
	}()
	return out
}
