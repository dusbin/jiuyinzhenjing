package main

import (
	"fmt"
	"math"
)
const file = "abc.txt"
const (
	abc = "test"
	def = 123
)
func consts(){
	const filename string = "abc.txt"
	const a,b = 3,4 // 不写类型  相当于文本替换，既可以是int 也可以是float64
	var c int
	c = int(math.Sqrt(a*a+b*b))
	fmt.Println(filename , c) // abc.txt 5
}
func enums(){
	/*
	const(
		cpp = 0
		java = 1
		python = 2
		golang = 3
	)
	*/
	const(
		cpp = iota
		java
		python
		golang
	)
	fmt.Println(cpp,java,python,golang)
}
func main() {
	consts()
	enums()
}
