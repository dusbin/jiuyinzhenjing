package main
import (
	"fmt"
	"os"
)
func main(){
	s:=""
	sep:=""
	for i:=1;i<len(os.Args);i++{
		s += sep +"[" +os.Args[i]+"]"
		sep = " "
	}
	fmt.Println(s)
	//hello_world()
}
func hello_world(){
	fmt.Println("Hello World")
}