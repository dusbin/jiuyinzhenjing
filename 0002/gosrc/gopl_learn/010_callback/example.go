package main
import(
	"fmt"
)
func main(){
	test1()
}
type Callback func(x,y int) int
func testCallback(x,y int,callback Callback) int{
	return callback(x,y)
}
func add(x,y int) int{
	return x+y
}
func sub(x,y int) int{
	return x-y
}
func mul(x,y int) int{
	return x*y
}
func div(x,y int) int{
	return x/y
}
func test1(){
	fmt.Println(testCallback(4,5,add))
	fmt.Println(testCallback(4,5,sub))
	fmt.Println(testCallback(4,5,mul))
	fmt.Println(testCallback(4,5,div))
}


