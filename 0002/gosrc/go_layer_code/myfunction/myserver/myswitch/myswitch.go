package myswitch
import(
	"os"
	"time"
	"fmt"
)
func MySwitch(input []byte)(flag int,output []byte){
	str_output:="HelloWord"
	flag = 1
	switch input[0]{
		case '1':
			str_output = fmt.Sprintf("%d年%d月%d日",time.Now().Year(),time.Now().Month(),time.Now().Day())
			flag = 0
		case '2':
			str_output = os.Getenv("USER")
			flag = 0
		default:
			str_output = "undefined"
			flag = 1
	}
	output = []byte(str_output)
	return
}