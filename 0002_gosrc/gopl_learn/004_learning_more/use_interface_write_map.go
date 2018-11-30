package main
import(
	"fmt"
)
type e interface{}
func mult2(f e) e{
	switch f.(type){
		case int://传进来的类型是int
			return f.(int)*2
		case string:
			return f.(string)+f.(string)+f.(string)+f.(string)
	}
	return f
}
func Map(n []e,f func(e) e) []e{
	m := make([]e,len(n))
	for k,v := range n{
		m[k] =f(v)
	}
	return m
}
//是由接口实现对int和string的支持
func test_1(){
	m := []e{1,2,3,4}
	s := []e{"a","b","c","d"}
	mf := Map(m,mult2)
	sf := Map(s,mult2)
	fmt.Println(mf)
	fmt.Println(sf)
}
func main(){
	test_1()
}