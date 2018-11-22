//+build server

package main
import (
	"encoding/binary"
	"bufio"
	"time"
	"net"
	"fmt"
	//"math/rand"
	"io"
)
func DoWork(){
	//out:=make(chan int)
	out:= RandonSource()
	listener,err:=net.Listen("tcp",":8000")
	if err != nil{
		panic(err)
	}
	go func(){
		defer listener.Close()
		conn,err:=listener.Accept()
		if err != nil{
			panic(err)
		}
		defer conn.Close()
		writer:= bufio.NewWriter(conn)
		WriterSink(writer,out)
		//defer writer.Flush()
	}()
	fmt.Println("server")
	time.Sleep(time.Minute*5)
}
func WriterSink(writer io.Writer,in <-chan int){
	for v:=range in{
		buffer:=make([]byte,8)
		fmt.Println(v)
		binary.BigEndian.PutUint64(buffer,uint64(v))
		writer.Write(buffer)
	}
}
func RandonSource()<-chan int{
	out:=make(chan int)
	go func(){//开启
		count := 0
		for{
			//out <-rand.Int()
			out <- count
			count++
			time.Sleep(time.Millisecond)
		}
		close(out)
	}()
	return out
}