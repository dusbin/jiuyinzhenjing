package pipeline
import(
	"net"
	"bufio"
)
//网络的sink 开启一个server goruntine进行等待任务
func NetWorkSink(addr string,in <-chan int){
	listener,err := net.Listen("tcp",addr)
	if err != nil{
		panic(err)
	}
	//go func 等待任务，不能讲listener在外面close，需要在go func中进行关闭
	go func(){
		defer listener.Close()
		conn,err:=listener.Accept()
		if err != nil{
			panic(err)
		}
		defer conn.Close()
		writer := bufio.NewWriter(conn)
		defer writer.Flush()
		WriterSink(writer,in)
	}()
}
//从addr读取数据，放到chan中
func NetWorkSource(addr string)<-chan int{
	out:=make(chan int)
	go func(){
		conn,err:=net.Dial("tcp",addr)
		if err != nil{
			panic(err)
		}
		r := ReaderSource(bufio.NewReader(conn),-1)//r是一个channel int
		for v:=range r{
			out<-v
		}
		close(out)
	}()
	return out
}