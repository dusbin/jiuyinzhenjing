package main
import (
    "fmt"
    "io/ioutil"
    "net"
)
func main() {
    localAddress, _ := net.ResolveTCPAddr("tcp4", "0.0.0.0:8080") //定义一个本机IP和端口。
    var tcpListener, err = net.ListenTCP("tcp", localAddress)       //在刚定义好的地址上进监听请求。
    if err != nil {
        fmt.Println("监听出错：", err)
        return
    }
    defer func() { //担心return之前忘记关闭连接，因此在defer中先约定好关它。
        tcpListener.Close()
    }()
    fmt.Println("正在等待连接...")
    var conn, err2 = tcpListener.AcceptTCP() //接受连接。
    if err2 != nil {
        fmt.Println("接受连接失败：", err2)
        return
    }
    var remoteAddr = conn.RemoteAddr() //获取连接到的对像的IP地址。
    fmt.Println("接受到一个连接：", remoteAddr)
    fmt.Println("正在读取消息...")
    var bys, _ = ioutil.ReadAll(conn) //读取对方发来的内容。
    fmt.Println("接收到客户端的消息：", string(bys))
    conn.Write([]byte("hello, Nice to meet you, my name is SongXingzhu")) //尝试发送消息。
    conn.Close()                                                          //关闭连接。
}