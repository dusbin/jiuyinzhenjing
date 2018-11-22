//+build plugin1

package main
import (
	"fmt"
)
func PrintTest(strInput string){
	fmt.Println("plugin1.so:",strInput)
}
