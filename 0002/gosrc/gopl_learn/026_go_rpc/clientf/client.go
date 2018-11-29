package main
import (
	"net/rpc"
	"log"
	"fmt"
)
type Args struct {
	A,B int
}
type Quotient struct {
	Quo,Rem int
}
func main(){
	client,err:=rpc.DialHTTP("tcp","127.0.0.1:1234")
	if err != nil {
		log.Fatal("dialing",err)
	}
	args := &Args{7,8}
	var reply int
	if err:= client.Call("Arith.Multiply",args,&reply); err != nil {
		log.Fatal("arith error:",err)
	}
	fmt.Println("Arith:",args.A,"*",args.B,"=",reply)
	quotient := new(Quotient)
	divCall := client.Go("Arith.Divide",args,quotient,nil)
	replyCall := <-divCall.Done
	if replyCall.Error != nil {
		log.Fatal("arith error:",replyCall.Error)
	}
	fmt.Println("Arith:",args.A,"/",args.B,"=",quotient.Quo,"...",quotient.Rem)

}