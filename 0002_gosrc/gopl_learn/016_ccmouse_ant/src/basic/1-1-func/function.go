package main

import "fmt"

func sum(in ...int) int{
	sum := 0
	for _,v := range in {
		sum += v
	}
	return sum
}
func avge(in ...int) int{
	sum := 0
	i := 0
	for _,v := range in {
		sum += v
		i++
	}
	return sum/i
}
func opt(f func(...int)int,in ...int) int {
	return f(in...)
}
func main() {
	fmt.Println(opt(sum,1,2,3,4,5))
	fmt.Println(opt(avge,1,2,3,4,5,6))
}
