package main

import (
	"fmt"
	"github.com/bytedance/go-tagexpr/validator"
	"reflect"
)

// 字节跳动实现的struct tag
// 实现了结构体标签表达式解释器，可应用于各种场景的请求参数校验
// 和json字段的导出类似
// https://www.oschina.net/news/104054/go-tagexpr-1-0-released?hmsr=toutiao.io&utm_medium=toutiao.io&utm_source=toutiao.io
/*
	主要特性：
		支持各种常用运算符
		支持访问数组、切片、字典的成员
		支持访问当前结构中任意字段
		支持访问嵌套字段、非导出字段等
		内置 len、sprintf、regexp 函数
		支持单表达式与多表达式两种定义模式
		提供了参数校验子包
*/
func test_1() {
   /*
     1.创建一个vd标签包含两个成员，vm和errfactory
     2.vm为一个标签名的map[string]Struct的结构体实例
     3.errfactory是一个构造错误信息的工厂函数
    */
	var vd = validator.New("vd")
	type InfoRequest struct {
		Name string `vd:"($!='Alice'||(Age)$==18)&&regexp("\\w")"`
		Age  int    `vd:"$>0"`
	}
	type testData struct {
		Name   string
		Age    int
		Result bool
	}
	data := []testData{
		{"Alice", 18, false},
		{"Alice", 17, true},
		{"Alice1", 18, true},
		{"Alice1", 17, true},
	}
	for _, test := range data {
		info := &InfoRequest{Name: test.Name, Age: test.Age}
      /*
        进行校验
        1.获取元素类型 判断是否为指针 是否为结构体 从map中获取对应的匹配规则，单例模式
       */
		fmt.Println(info, "you got", vd.Validate(info) == nil, "you want", test.Result)
	}
	/*
		&{Alice 18} you got false you want false
		&{Alice 17} you got true you want true
		&{Alice1 18} you got true you want true
		&{Alice1 17} you got true you want true
	*/
}

// https://blog.csdn.net/benben_2015/article/details/79807792
// 自己实现一个 struct tag
func test_2() {
	const tagName = "Testing"
	type Info struct {
		Name string `Testing:"-"`
		Age  int    `Testing:"age,min=17,max=60"`
		Sex  string `Testing:"sex,required"`
	}
	info := Info{
		Name: "benben",
		Age:  23,
		Sex:  "male",
	}
	t := reflect.TypeOf(info)
	fmt.Println("Type:", t.Name())
	fmt.Println("Kind:", t.Kind())

	// 每个字段
	for i := 0; i < t.NumField(); i++ {
		field := t.Field(i)
		tag := field.Tag.Get(tagName)
		fmt.Println(i+1, field.Name, field.Type.Name(), tag)
	}
	/*
		Type: Info
		Kind: struct
		1 Name string -
		2 Age int age,min=17,max=60
		3 Sex string sex,required
	*/
}

// https://blog.csdn.net/culminate_in/article/details/71081009
const (
	English = "en_US"
	Chinese = "zh_CN"
)

type resume struct {
	Name string `json:"name" en_US:"My Name" zh_CN:"我的名字"`
	Age  int    `json:"age" en_US:"My Age" zh_CN:"我的年龄"`
}

func findDoc(langure string, stru interface{}) map[string]string {
	t := reflect.TypeOf(stru).Elem()
	doc := make(map[string]string)
	for i := 0; i < t.NumField(); i++ {
		doc[t.Field(i).Tag.Get("json")] = t.Field(i).Tag.Get(langure)
	}
	return doc
}
func test_3() {
	type test struct {
		langure string
	}
	data := []test{
		{English},
		{Chinese},
	}
	var stru resume
	for _, test := range data {
		doc := findDoc(test.langure, &stru)
		fmt.Println(test.langure, doc["name"], doc["age"])
	}
	/*
		en_US My Name My Age
		zh_CN 我的名字 我的年龄
	*/
}
func main() {
	test_1()
	test_2()
	test_3()
}
