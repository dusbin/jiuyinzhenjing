package main

import (
	"fmt"
)

func test_01() {
	defer func() {
		if ok := recover(); ok != nil {
			fmt.Println("recover")
		}
	}()
	panic("error")
}
func main() {
	test_01()
}
