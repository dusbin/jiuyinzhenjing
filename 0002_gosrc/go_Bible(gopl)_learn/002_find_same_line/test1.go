//cat 1|go run test.go
package main
import (
	"bufio"
	"fmt"
	"os"
)
func main(){
	counts := make(map[string]int)
	files := os.Args[1:]
	if len(files) == 0 {
		countLines(os.Stdin,counts)
	}else{
		for _,arg:=range files {
			f,err:= os.Open(arg)
			if err != nil{
				fmt.Println("Open",arg,"error")
				continue
			}
			countLines(f,counts)
			f.Close()
		}
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