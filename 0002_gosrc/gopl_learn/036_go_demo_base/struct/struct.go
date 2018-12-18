package main

import (
	"fmt"
)

/*
	https://studygolang.com/articles/16917
	为了实现面向对象的继承，允许struct间使用匿名引入
	的方式实现对象属性方法的组合
	注意事项
		1. 使用匿名引入的方式来组合其他struct
		2. 默认优先调用外层方法
		3. 可以指定匿名struct以调用内层方法
*/
type People struct{}
type People2 struct{}

func (p *People) ShowA() {
	fmt.Println("ShowA")
	p.ShowB()
}
func (p *People) ShowB() {
	fmt.Println("ShowB")
	//p.ShowB()
}
func (p *People) ShowC() {
	fmt.Println("People: ShowC")
}
func (p *People) ShowD() {
	fmt.Println("People: ShowD")
}
func (p *People2) ShowD() {
	fmt.Println("People2: ShowD")
}

type Teacher struct {
	People  // 组合People
	People2 // 组合People2
}

func (t *Teacher) ShowB() {
	fmt.Println("teacher :ShowB")
}
func (t *Teacher) ShowC(arg string) {
	fmt.Println(arg)
}
func test_01() {
	t := Teacher{}
	t.ShowA()
	t.ShowB()
	t.People.ShowB()
	t.ShowC("test")
	t.People.ShowC()
	// 因为组合方法多次调用showD，所以调用时必须显示指定匿名方法
	t.People2.ShowD()
	t.People.ShowD()
}
func main() {
	test_01()
}
