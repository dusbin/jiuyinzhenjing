package main
import(
	"fmt"
	"os"
)
func main(){
    //使用os.Getenv获取系统中的环境变量，通过key获取value
	fmt.Println("user:[",os.Getenv("USER"),"]")
	fmt.Println("pwd :[",os.Getenv("PWD"),"]")
	currentPath,_ :=os.Getwd()
	fmt.Println("GetWd()",currentPath)
}