package heapsort

import (
	"fmt"
)

func Sort(input []int) {
	i_len_half := (len(input) - 1) / 2
	flag := 1
	for flag == 1 {
		flag = 0
		for i := 0; i < i_len_half; i++ {
			flag1 := 0
			if 2*i+2 > len(input) {
				flag1 = min(input[i], input[2*i+1], 9999)
			} else {
				flag1 = min(input[i], input[2*i+1], input[2*i+2])
			}
			if flag1 == 2 {
				input[i], input[2*i+1] = input[2*i+1], input[i]
				flag = 1
			}
			if flag1 == 3 {
				input[i], input[2*i+2] = input[2*i+2], input[i]
				flag = 1
			}
		}
	}
}
func Print(input []int) {
	i_len_half := (len(input) - 1) / 2
	for i := 0; i < i_len_half; i++ {
		fmt.Println(input[i], input[2*i+1], input[2*i+2])
	}
}
func min(a, b, c int) (flag int) {
	m := a
	flag = 1
	if b < m {
		m = b
		flag = 2
	}
	if c < m {
		m = c
		flag = 3
	}
	return
}
