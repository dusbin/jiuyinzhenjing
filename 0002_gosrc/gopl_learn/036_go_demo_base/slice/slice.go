package main

import "fmt"

func main() {
	d := []interface{}{}
	d = append(d, "hello")
	d = append(d, 1)
	d = append(d, "world")
	m := make(map[string]string)
	m["name"] = "duzhengbin"
	m["tel"] = "17782502284"
	d = append(d, m)
	i := []int{1, 2, 3, 4, 5}
	d = append(d, i)
	fmt.Println(d)
}
