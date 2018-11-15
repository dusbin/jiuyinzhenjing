package mysocket
import(
	"net"
	"fmt"
	"io"
	"../myswitch"
)
const RECV_BUF_LEN = 1024
func MySocket(){
	listen,err := net.Listen("tcp","0.0.0.0:6666")
	if err != nil{
		panic("error listener:"+err.Error())
	}
	fmt.Println("Starting the server")
	for{
		conn,err := listen.Accept()
		if err != nil{
			panic("error accept:"+err.Error())
		}
		fmt.Println("Accepted the Connection:",conn.RemoteAddr())
		go working(conn)
	}
}
func working(conn net.Conn){
	buf_input := make([]byte,RECV_BUF_LEN)
	buf_output := make([]byte,RECV_BUF_LEN)
	defer conn.Close()
	for{
		_,err := conn.Read(buf_input)
		switch err{
			case nil:
				buf_output = myswitch.MySwitch(buf_input)
				conn.Write(buf_output[0:])
			case io.EOF:
				fmt.Println("Warning:End of Data:",err)
				return
			default:
				fmt.Println("Error:Reading Data:",err)
				return
		}
	}
}