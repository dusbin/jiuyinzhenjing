package pipeline
import(
	"sort"
)
//分块读取数据
func ArraySource(a...int/*a...int 是一个可变参数*/)<-chan int/*chan int 返回一个int的channel,调用者只能拿数据，函数只能写数据*/{
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
//排序
func InMemSort(in <- chan int)<-chan int{//in 相对于当前这个函数是一个只接收输入的channel，返回值是一个只输出的channel
	out:=make(chan int)
	go func(){
		a:= []int{}
		for v:= range in{//从channel中取数据放到int类型的slice a中
			a = append(a,v)
		}
		sort.Ints(a)//排序
		for _,v:=range a{
			out <- v //将元素一个一个的通channel传出去
		}
		close(out)//传完之后关闭channel
	}()
	return out
}
//归并
func Merge(in1,in2 <-chan int)<-chan int{
	out:=make(chan int)
	go func(){
		v1,ok1:=<-in1
		v2,ok2:=<-in2
		for ok1 || ok2{//还有数据
			if !ok2 ||(ok1&&v1 <= v2){
				out <- v1
				v1,ok1 = <-in1
			}else{
				out<-v2
				v2,ok2 = <-in2
			}
		}
		close(out)//没有数据进行关闭
	}()
	return out
}