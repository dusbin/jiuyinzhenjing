package lib
import (
	"fmt"
)
// 初始化函数
func init() {
	fmt.Println("sum.init")
}
// 求和
func Sum(x ...int) int{
	n := 0
	for _,i := range x {
		n += i
	}
	return n
}