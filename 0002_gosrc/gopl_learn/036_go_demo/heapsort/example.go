package main

import (
	"./heapsort"
	"fmt"
)

/*
	堆排序
*/
func test_01() {
	//a := []int{3,2,5,4,1,6,8,7}
	a := []int{3, 2, 5, 4, 1, 6, 8, 7, 11, 123, 12, 14, 21, 4, 234, 1245, 12, 223, 423}
	fmt.Println(a)
	heapsort.Sort(a)
	fmt.Println(a)
	heapsort.Print(a)
}

/*
	堆排序查找N大
*/
func test_02() {
	a := []int{0, 0, 0, 0, 0, 0, 0, 0}
	b := []int{12, 1, 3, 123, 111, 234, 1232, 121, 42, 1212, 2323, 13, 16, 12}
	flag := 0
	for _, v := range b {
		if flag < len(a) {
			a[flag] = b[flag]
			flag += 1
		} else if v > a[0] {
			heapsort.Sort(a)
			a[0] = v
		}
	}
	heapsort.Sort(a)
	heapsort.Sort(b)
	fmt.Println(b)
	fmt.Println(a)
}
func main() {
	test_01()
	test_02()
}
