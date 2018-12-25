package main

import (
	"fmt"
)

type double float64

func (a double) IsEqual(b double) bool {
	var r = a - b
	if r == 0.0 {
		return true
	} else if r < 0.0 {
		return r > -0.0001
	}
	return r < 0.0001
}

/*
	匿名函数的使用
*/
func test_func1() {
	fmt.Println("test_func1")
	a := 1
	b := "hello"
	c := func(i int, s string) int {
		fmt.Println(i, s)
		i += 1
		return i
	}(a, b)
	fmt.Println(a, b, c)
}
func main() {
	test_digui(9)
	test_more_return()
	test_jiecheng()
	test_defer()
	test_func()
	test_IsEqual()
	test_func1()
}
func test_IsEqual() {
	var a double = 1.9999
	var b double = 1.99998
	fmt.Println(a.IsEqual(b))
}
func test_func() {
	a := func() {
		fmt.Println("hello")
	}
	a()
	fmt.Println("a:", a)
}
func test_defer() {
	for i := 0; i < 5; i++ {
		defer fmt.Println(i) //打印 4 3 2 1 0
	}
}
func test_digui(in int) {
	if in == 0 {
		return
	}
	fmt.Println(in)
	test_digui(in - 1)
	//fmt.Println(in)

}
func test_more_return() {
	a := []byte{'1', '2', '3', '4'}
	x := 0
	for i := 0; i < len(a); {
		x, i = nextInt(a, i)
		fmt.Println(x)
	}
}
func test_jiecheng() {
	fmt.Println("4:", Factorial(4))
	fmt.Println("5:", Factorial_1(5))
	val, pos := fib(5)
	fmt.Println("fib:", pos, ",", val)
}
func nextInt(b []byte, i int) (int, int) {
	x := 0
	for ; i < len(b); i++ {
		x = x*10 + int(b[i]) - '0'
	}
	return x, i
}

//阶乘的实现1*2*3*4....
func Factorial(x int) int {
	if x == 0 {
		return 1
	} else {
		return x * Factorial(x-1)
	}
}

//阶乘的实现1*2*3*4...
func Factorial_1(x int) (result int) {
	if x == 0 {
		result = 1
	} else {
		result = x * Factorial_1(x-1)
	}
	return
}

//斐波拉契
func fib(n int) (val, pos int) {
	if n == 0 {
		val = 1
		pos = 0
	} else if n == 1 {
		val = 1
		pos = 1
	} else {
		v1, _ := fib(n - 1)
		v2, _ := fib(n - 2)
		val = v1 + v2
		pos = n
	}

	return
}
