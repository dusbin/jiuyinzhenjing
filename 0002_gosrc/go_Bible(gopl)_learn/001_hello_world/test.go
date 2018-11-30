package main
import (
	"fmt"
	"os"
	"strings"
)
func main(){
	s,sep:="",""
	for _,arg := range os.Args[:]{
		s += sep +"[" +arg+"]"
		sep = " "
	}
	fmt.Println(s)
	fmt.Println(strings.Join(os.Args[:]," "))
	fmt.Println(os.Args[:])
}