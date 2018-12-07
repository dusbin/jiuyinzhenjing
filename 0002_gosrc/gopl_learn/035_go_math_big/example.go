package main

import (
	"fmt"
	"math/big"
)
func main(){
	i1 := new(big.Int)
	i2 := new(big.Int)
	i3 := new(big.Int)
	fmt.Sscan("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890",i1)
	fmt.Sscan("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890",i2)
	fmt.Println(i1)
	fmt.Println(i2)
	i3.Sub(i1,i2) // -
	fmt.Println(i3)
	i3.Add(i1,i2) // +
	fmt.Println(i3)
	i3.Div(i1,i2) // /
	fmt.Println(i3)
	i3.Mul(i1,i2) // *
	fmt.Println(i3)
	i3.Sqrt(i1) // sqrt
	fmt.Println(i3) 
}