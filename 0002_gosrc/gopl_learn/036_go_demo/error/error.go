package main

import (
	"fmt"
)

func main() {
	if res, err3 := checkAge(-12); err3 != nil {
		fmt.Println(err3.Error())
		fmt.Println(err3)
	} else {
		fmt.Println(res)
	}
	if res, err3 := checkAge(12); err3 != nil {
		fmt.Println(err3.Error())
		fmt.Println(err3)
	} else {
		fmt.Println(res)
	}
}
func checkAge(age int) (string, error) {
	if age < 0 {
		err := fmt.Errorf("您的年龄输入是：%d，该数值为负数，有错误！", age)
		return "", err
	} else {
		return fmt.Sprintf("您的年龄输入是：%d", age), nil
	}
}
