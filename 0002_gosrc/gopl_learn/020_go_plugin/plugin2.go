//+build plugin2

package main
import (
	"fmt"
)
func PrintTest(strInput string){
	fmt.Println("plugin2.so:",strInput)
}