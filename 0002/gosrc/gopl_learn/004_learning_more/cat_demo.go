package main
import(
	"fmt"
	"io"
	"os"
	"bufio"
	"flag"
)
var numberFlag = flag.Bool("n",false,"number each line")
func main(){
	test_1()
}
func test_1(){
	flag.Parse()
	if flag.NArg()==0{
		cat(bufio.NewReader(os.Stdin))
	}
	for i:=0;i<flag.NArg();i++{
		f,e:=os.Open(flag.Arg(i))
		if e!=nil{
			//fmt.Fprintf(os.Stderr,"%s:error reading from %s:%s\n",os.Args[0],flag.Args(i),e.String())
			continue
		}
		cat(bufio.NewReader(f))
	}
}
func cat(r *bufio.Reader){
	i:=1
	for{
		buf,e:=r.ReadBytes('\n')
		if e == io.EOF{
			break
		}
		if *numberFlag {
			fmt.Fprintf(os.Stdout,"%5d %s",i,buf)
		}else{
			fmt.Fprintf(os.Stdout,"%s",buf)
		}
	}
}


