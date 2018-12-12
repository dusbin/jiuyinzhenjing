package main

import (
	"fmt"
)

/*
	多个defer的执行顺序为后进先出
	defer return 返回值的执行逻辑是
	return最先执行，
	return负责将结果写入返回值中，
	接着defer开始执行收尾工作
	最后函数携带当前返回值退出
	https://blog.csdn.net/qq_22063697/article/details/74892728
*/
func test() int {
	i := 0
	defer fmt.Println("defer1 i:", i) // 7:0
	fmt.Println("body1 i:", i)        // 1 ：0
	defer func() {
		i++                             // 5
		fmt.Println("defer func i:", i) // 6：2
	}()
	i++                               //2
	defer fmt.Println("defer2 i:", i) // 4：1
	fmt.Println("body2 i:", i)        // 3:1
	return i
}
func main() {
	fmt.Println("main1 i:", test()) // 8：1
}
