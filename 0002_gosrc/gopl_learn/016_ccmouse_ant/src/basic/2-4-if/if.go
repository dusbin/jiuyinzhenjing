package main

import (
	"fmt"
	"io/ioutil"
)
func eval(a,b int,op string) int {
	var result int
	switch op {
	case "+":
		result = a + b
	case "-":
		result = a - b
	case "*":
		result = a * b
	case "/":
		result = a / b
	default:
		panic("unsupported operator:" + op)
		}
	return result

}
func grade(score int) string {
	g := ""
	switch {
	case score < 0 || score > 100:
		panic(fmt.Sprintf("Wrong score:%d",score))
	case score < 60:
		g = "F"
	case score < 80:
		g = "C"
	case score < 90:
		g = "B"
	case score <= 100:
		g = "A"
	}
	return g
}
func main() {
	const filename = "abc.txt"
	contents,err := ioutil.ReadFile(filename)
	if err != nil {
	// if contents,err := ioutil.ReadFile(filename);err != nil {
		fmt.Println(err)
	}else {
		fmt.Println(contents)
	}
	fmt.Println(eval(1,2,"-"))
	fmt.Println(grade(1),grade(0),grade(100)/*,grade(101),grade(-1)*/)
}
