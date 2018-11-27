package main
func main(){
	var sig = make(chan struct{})
	close(sig) //必须使用sig进行close，才能执行<-sig,否则会死锁
	<-sig
}