package main

import (
	"encoding/json"
	"fmt"
)

//Student 结构体
type Student struct {
	Name string
	Age  int
}

//Person JSON字段标签
type Person struct {
	Name        string `json:"username"` //自定义名称
	Age         int
	Gender      bool `json:",omitempty"` //如果该字段值为空，就不会输出到JSON串中
	Profile     string
	OmitContent string `json:"-"`       //字段不会输出到JSON
	Count       int    `json:",string"` //自定义名称
}

func main() {
	//结构体序列化为JSON
	s := Student{"黄哲", 20}
	if bs, err := json.Marshal(s); err != nil {
		panic(err)
	} else {
		fmt.Println(string(bs)) //{"Name":"黄哲","Age":20}
	}

	//切片序列化为JSON
	s1 := Student{"张三", 21}
	s2 := Student{"李四", 22}
	s3 := Student{"王五", 23}
	slice := []Student{s1, s2, s3}
	bs, err := json.Marshal(slice)
	if err != nil {
		panic(err)
	} else {
		fmt.Println(string(bs)) //[{"Name":"张三","Age":21},{"Name":"李四","Age":22},{"Name":"王五","Age":23}]
	}

	//map序列化为JSON
	var m = make(map[string]string)
	m["Go"] = "No.1"
	m["C++"] = "No.2"
	m["C"] = "No.3"
	m["C#"] = "No.4"
	if bs, err := json.Marshal(m); err != nil {
		panic(err)
	} else {
		fmt.Println(string(bs)) //{"C":"No.3","C#":"No.4","C++":"No.2","Go":"No.1"}
	}
	//JSON字段标签
	var p = &Person{
		Name:        "HZ",
		Age:         21,
		Gender:      true,
		Profile:     "from shandong",
		OmitContent: "OmitContent",
	}
	if bs, err := json.Marshal(p); err != nil {
		panic(err)
	} else {
		fmt.Println(string(bs))
	}

}
