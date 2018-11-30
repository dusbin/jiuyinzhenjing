package main
import (
	"./myserver"
	"./myclient"
	"os"
)
func main(){
	if os.Args[0] == "myserver"{
		myserver.MyServer()
	}
	if os.Args[0] == "myclient"{
		myclient.MyClient()
	}
	
}