package pipeline
import(
	"sort"
	"io"
	"encoding/binary"
	"math/rand"
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
		//排序完成后才开始发送数据
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
	go func(){//同时从两个channel中获取数据，两个channel的数据量不一定一样
		v1,ok1:=<-in1
		v2,ok2:=<-in2
		for ok1 || ok2{//还有数据
			/*
			 * 发送v1的条件（即if分支） 
			 *	 channel 2没数据，channel 1有数据
			 *   channel 2有数据channel 1有数据，且channel1的数据小于channel2的数据
			 * 发送v2的数据（即else分支）
			 */
			if !ok2 ||(ok1&& v1 <= v2){
				out <- v1
				v1,ok1 = <-in1 //送完后更新v1的数据
			}else{
				out <- v2
				v2,ok2 = <-in2 //送完后更新v2的数据
			}
		}
		close(out)//没有数据进行关闭
	}()
	return out
}
//读取数据 增加最多读取chunkSize大小的数据 -1 全部度
func ReaderSource(reader io.Reader,chunkSize int) <-chan int{
	out:=make(chan int)
	go func(){
		buffer :=make([]byte,8)//64位的int
		bytesRead := 0
		for{
			n,err:= reader.Read(buffer)
			bytesRead += n //每读一次 自增
			if n > 0 {
				v:= int(binary.BigEndian.Uint64(buffer))//从buffer中进行大端读，转为uint64位
				out <- v
			}
			if err != nil || (chunkSize != -1 && bytesRead > chunkSize){//出错或者chunkSize不等于-1并且bytesRead大于chunkSize
				break
			}
		}
		close(out)//关闭channel
	}()
	return out
}
//写文件
func WriterSink(writer io.Writer,in <-chan int){
	for v:=range in{
		buffer := make([]byte,8)
		binary.BigEndian.PutUint64(buffer,uint64(v))
		writer.Write(buffer)
	}
}
//随机数据源
func RandomSource(count int) <-chan int{
	out:=make(chan int)
	go func(){
		for i:=0;i<count;i++{
			out  <- rand.Int()
		}
		close(out)
	}()
	return out
}
//N路归并
func MergeN(inputs...<-chan int) <-chan int{//输入为chan int类型的可变参数，输出为chan的int
	if len(inputs) == 1{//递归结束，当inputs为一路时
		return inputs[0]
	}
	m:=len(inputs)/2 //将N路一分为二
	//merge inputs[0..m) and inputs[m..end)
	return Merge(
		MergeN(inputs[:m]...)/*前半部*/,
		MergeN(inputs[m:]...)/*后半部分*/)
}