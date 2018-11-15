package main
import(
	"net"
	"fmt"
	"io"
)
const RECV_BUF_LEN = 1024
func main(){
	listener,err := net.Listen("tcp","0.0.0.0:6666")//监听端口6666
	if err!=nil{
		panic("error listener:"+err.Error())
	}
	fmt.Println("Staring the server")
	for{
		conn,err := listener.Accept()//接受连接
		if err != nil{
			panic("Error accpet:"+err.Error())
		}
		fmt.Println("Accepted the Connection:",conn.RemoteAddr())
		go EchoServer(conn)
	}
}
func EchoServer(conn net.Conn){
	buf:=make([]byte,RECV_BUF_LEN)
	defer conn.Close()
	for{
		n,err := conn.Read(buf)
		switch err{
			case nil:
				conn.Write(buf[1:n])
			case io.EOF:
				fmt.Println("Warning:End of Data:",err)
				return
			default:
				fmt.Println("Error:Reading data:",err)
				return
		}
	}
}