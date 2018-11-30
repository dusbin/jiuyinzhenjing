//cat 1|go run test.go
package main
import (
	//"bufio"
	"fmt"
	"os"
	"io/ioutil"
	"strings"
)
func main(){
	counts := make(map[string]int)
	for _,filename:=range os.Args[1:] {
		data,err:= ioutil.Open(filename)
		if err != nil{
			fmt.Println("Open",arg,"error")
			continue
		}
		countLines(f,counts)
		f.Close()
	}
	
	for line, n := range counts {
		if n > 1{
			fmt.Println(n,":",line)
		}
	}
}
func countLines(f *os.File,counts map[string]int){
	input := bufio.NewScanner(f)
	for input.Scan() {
		counts[input.Text()]++
	}
}