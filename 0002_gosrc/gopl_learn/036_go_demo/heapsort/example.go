package main
import (
	"./heapsort"
	"fmt"
)
func main(){
	//a := []int{3,2,5,4,1,6,8,7}
	a := []int{3,2,5,4,1,6,8,7,11,123,12,14,21,4,234,1245,12,223,423}
	fmt.Println(a)
	heapsort.Sort(a)
	fmt.Println(a)
	heapsort.Print(a)
}