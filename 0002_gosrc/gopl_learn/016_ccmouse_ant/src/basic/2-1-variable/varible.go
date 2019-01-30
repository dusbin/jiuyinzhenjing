package main

import "fmt"
// 包内部变量 只能用 var ；不能用 aa := 3
/*
var aa = 3
var ss = "kkk"
var bb = true
*/
var (
	aa = 3
 	ss = "kkk"
 	bb = true
)
func variableZeroValue(){
	var a int // 默认是0
	var s string // 默认是空串
	fmt.Printf("%q\n",s)
	fmt.Println(a,s)
}
func variableInitialValue(){
	var a, b int = 3,4 // 可以赋多值
	var s string = "abc"
	fmt.Println(a,s,b)
}
// 不规定类型
func variableTypeDeduction(){
	var a,b,c,s = 3,4,true,"def"
	fmt.Println(a,b,c,s)
}
func variableShorter(){
	a,b,c,s := 3,4,true,"def"
	fmt.Println(a,b,c,s)
	b = 5 // 不能在用 :=  定义时使用 := 这是赋值
	fmt.Println(a,b,c,s)
}
func main() {
	fmt.Println("hello world")
	variableZeroValue()
	variableInitialValue()
	variableTypeDeduction()
	variableShorter()
}
