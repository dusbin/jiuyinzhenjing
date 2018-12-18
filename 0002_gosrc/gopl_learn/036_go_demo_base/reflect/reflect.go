package main

import (
	"encoding/json"
	"fmt"
	"reflect"
)

/*
	https://studygolang.com/articles/16916
	数据类型在运行期的识别，获取和判断
*/
func test_01() {
	var data interface{} = "hello"
	strvalue, ok := data.(string)
	if ok {
		fmt.Println(strvalue, " is string")
	}
	var f interface{}
	b := []byte(`[{"name":"wednesday","age":6,"parents":["Gomez","Morticia"]}]`)
	json.Unmarshal(b, &f) // 结构化
	data1, ok1 := f.([]interface{})
	if ok1 {
		fmt.Println(data1, " is data")
	}
}

/*
	https://studygolang.com/articles/12348?fr=sidebar
*/
func test_02() {
	var num float64 = 1.2345
	fmt.Println("type: ", reflect.TypeOf(num))
	fmt.Println("value: ", reflect.ValueOf(num))
}
func main() {
	//test_01()
	test_02()
}
