//+build client

package main
import (
	"encoding/binary"
	"bufio"
	"fmt"
	"net"
	"io"
	"time"
)
func DoWork(){
	go func(){
		conn,err:=net.Dial("tcp",":8000")
		if err != nil{
			panic(err)
		}
		defer conn.Close()
		r := ReaderSource(bufio.NewReader(conn),-1)
		for v:= range r{
			fmt.Println(v)
		}
	}()
	fmt.Println("client")
	time.Sleep(time.Minute*5)
}
func ReaderSource(reader io.Reader,chunkSize int)<-chan int{
	out:=make(chan int)
	go func(){
		buffer:=make([]byte,8)
		bytesRead:=0
		for{
			n,err:=reader.Read(buffer)
			bytesRead += n
			if n > 0{
				v:=int(binary.BigEndian.Uint64(buffer))
				fmt.Println(v)
				out <- v
			}
			if err != nil || (chunkSize != -1 && bytesRead>= chunkSize){
				break
			}
		}
		close(out)
	}()
	return out
}