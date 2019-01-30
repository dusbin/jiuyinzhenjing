package main

import (
	"fmt"
	"math"
	"math/cmplx"
)

// 欧拉公式 e的iπ次方+1 = 0
func euler(){
	c := 3 + 4i // c 是复数
	fmt.Println(cmplx.Abs(c)) //cmplx.Abs 取模 结果为5
	fmt.Println(cmplx.Pow(math.E, 1i*math.Pi) + 1) // 存在误差(0+1.2246467991473515e-16i)
	fmt.Println(cmplx.Exp(1i * math.Pi) + 1)//(0+1.2246467991473515e-16i)
	fmt.Printf("%.20f\n",cmplx.Exp(1i * math.Pi) + 1) //(0.00000000000000000000+0.00000000000000012246i)
}
func triangle(){
	var a,b int = 3,4
	var c int
	c = int(math.Sqrt(float64(a*a + b*b))) //math.Sqrt 参数为float64 需要显示转换 强制类型转换 go语言不支持隐式转换
	fmt.Println(c)
}
func main() {
	euler()
	triangle()
}
