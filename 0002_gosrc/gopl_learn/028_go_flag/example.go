package main
import (
	"flag"
	"fmt"
	"os"
)
var (
	h,v,V,t,T bool
	q *bool
	s,p string

)
func init(){ //进行初始化
	fmt.Println("init....")
	/* 
	  BoolVar函数
	  	第一个参数变量的地址
		第二个参数变量的值，即程序输入的 -h -v 除去-
		第三个参数为默认值，
		第四个参数为描述
	 */
	flag.BoolVar(&h,"h",false,"this help")
	flag.BoolVar(&v,"v",false,"show version and exit")
	flag.BoolVar(&V,"V",false,"show version and configure options then exit")
	flag.BoolVar(&t,"t",false,"test configure amd exit")
	flag.BoolVar(&T,"T",false,"test configure,dump it and exit")
	/*
	  Bool函数
		第一个参数为变量的值，即程序输入的-q等
		第二个参数为默认值，
		第三个参数为描述
		返回值为*bool 即bool类型的地址
	 */
	q = flag.Bool("q",false,"suppress non-errors message during configure")
	/*
	  StringVar函数
		第一个参数变量的地址
		第二个参数变量的值，即程序输入的 -h -v 除去-
		第三个参数为默认值，
		第四个参数为描述
	 */
	flag.StringVar(&s,"s","","send `signal` to a master")
	flag.StringVar(&p,"p","user/local/nginx","set `prefix` path")
	// Usage函数赋值
	flag.Usage = usage
}
func main(){
	flag.Parse()
	if h {
		flag.Usage()
	}
	if v {
		fmt.Println("vvvv")
		flag.Usage()
	}
}
func usage(){
	fmt.Fprintf(os.Stderr,`nginx version:nginx/1.10.0
Usage: nginx [-hvVtTq] [-s signal] [-p prefix]

Options:
`)
	// 打印所有的信息
	flag.PrintDefaults()
}