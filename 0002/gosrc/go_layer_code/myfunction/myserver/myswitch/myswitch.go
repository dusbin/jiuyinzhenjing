package myswitch
import(
	"os"
	"time"
	"fmt"
)
func MySwitch(input []byte)(output []byte){
	str_output:="HelloWord"
	switch input[0]{
		case '1':
			str_output = fmt.Sprintf("%d年%d月%d日",time.Now().Year(),time.Now().Month(),time.Now().Day())
		case '2':
			str_output = os.Getenv("USER")
		default:
			str_output = "undefined"
	}
	output = []byte(str_output)
	return
}