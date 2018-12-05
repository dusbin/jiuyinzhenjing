package main
import (
	"log"
	"time"
	"sync"
	"sync/atomic"
	"reflect"
	"fmt"
	"unsafe"
	"math/rand"
)
// https://studygolang.com/articles/9446
func test_01(){		// sizeof	Alignof(对齐倍数)	Offset(偏移量)
	fmt.Println("test_01")
	var x struct {	// 32		8
		a bool		// 1		1					0
		//d bool		// 1		1					1 将空洞填上了
		//f bool		//
		b int16		// 2		2					2
		c []int		// 24		8					8
	}
	fmt.Printf("%-30s%-30s%-30s%-30s\n","Row","Sizeof","Alignof(对齐倍数)","Offsetof(偏移量)")
	fmt.Printf("%-30s%-30d%-30d%-50s\n","x",unsafe.Sizeof(x),unsafe.Alignof(x),"")
	fmt.Printf("%-30s%-30d%-30d%-50d\n","x.a",unsafe.Sizeof(x.a),unsafe.Alignof(x.a),unsafe.Offsetof(x.a))
	//fmt.Printf("%-30s%-30d%-30d%-50d\n","x.d",unsafe.Sizeof(x.d),unsafe.Alignof(x.d),unsafe.Offsetof(x.d))
	//fmt.Printf("%-30s%-30d%-30d%-50d\n","x.f",unsafe.Sizeof(x.f),unsafe.Alignof(x.f),unsafe.Offsetof(x.f))
	fmt.Printf("%-30s%-30d%-30d%-50d\n","x.b",unsafe.Sizeof(x.b),unsafe.Alignof(x.b),unsafe.Offsetof(x.b))
	fmt.Printf("%-30s%-30d%-30d%-50d\n","x.c",unsafe.Sizeof(x.c),unsafe.Alignof(x.c),unsafe.Offsetof(x.c))
	fmt.Println(reflect.TypeOf(x)) // struct { a bool; b int16; c []int }
}
func Float64bits(f float64) uint64{
	fmt.Println(reflect.TypeOf(&f)) // *float64
	fmt.Println(reflect.TypeOf(unsafe.Pointer(&f))) // unsafe.Pointer
	fmt.Println(reflect.TypeOf((*uint64)(unsafe.Pointer(&f)))) // *uint64
	return *(*uint64)(unsafe.Pointer(&f))
}
func test_02(){
	fmt.Println("test_02")
	fmt.Printf("%#016x\n",Float64bits(1.0)) // 0x3ff0000000000000 
	fmt.Println(reflect.TypeOf(Float64bits)) // func(float64) uint64
}
func test_03(){
	fmt.Println("test_03")
	var x struct {
		a bool
		b int16
		c []int
	}
	fmt.Println("uintptr(unsafe.Pointer(&x)):",uintptr(unsafe.Pointer(&x)))// x的绝对地址
	fmt.Println("uintptr(unsafe.Pointer(&x)) + unsafe.Offsetof(x.b):",uintptr(unsafe.Pointer(&x)) + unsafe.Offsetof(x.b))// x.b的绝对地址
	pb := (*int16)(unsafe.Pointer(
		uintptr(unsafe.Pointer(&x)) + unsafe.Offsetof(x.b))) // pb 是一个地址，指向x.b,x.b类型为int16 所以需要强制转换为 *int16
	*pb = 42 //对pb地址上的元素设值为42 pb是x.b的地址，即修改x.b的值
	fmt.Println(x.b) // "42"
}
func test_04(){
	fmt.Println("test_04")
	var x struct {
		a bool
		b int16
		c []int
	}
	fmt.Println("uintptr(unsafe.Pointer(&x)):",uintptr(unsafe.Pointer(&x)))// x的绝对地址
	fmt.Println("uintptr(unsafe.Pointer(&x)) + unsafe.Offsetof(x.b):",uintptr(unsafe.Pointer(&x)) + unsafe.Offsetof(x.b))// x.b的绝对地址
	tmp := uintptr(unsafe.Pointer(&x)) + unsafe.Offsetof(x.b)// tmp 是一个地址，指向x.b
	pb := (*int16)(unsafe.Pointer(tmp)) // 提示 possible misuse of unsafe.Poniter
	*pb = 42 //对pb地址上的元素设值为42 pb是x.b的地址，即修改x.b的值
	fmt.Println(x.b) // "42"
}
func test_05(){
	fmt.Println("test_05")
	// 转换可能无意义，但是是安全和合法的
	var n int64 = 5
	var pn = &n
	var pf = (*float64)(unsafe.Pointer(pn))
	fmt.Println(*pf)
	*pf = 3.1415
	fmt.Println(n)
	// uintptr和unsafe.Pointer互相转换
	a := [4]int{0,1,2,3}
	p := &a[1] //内存地址
	p1 := unsafe.Pointer(p)
	p2 := uintptr(p1) // p2的内容为p1地址的uint化
	p3 := unsafe.Pointer(p2)//获取地址也就是p1的地址 
	// p1的地址和p3的地址相同，等于p2的值
	fmt.Printf("%p %d\n",p1,*(*int)(p1))
	fmt.Printf("%p 0x%x\n",&p2,p2)
	fmt.Printf("%p %d\n",p3,*(*int)(p3))
}
func test_06(){
	fmt.Println("test_06")
	type MyInt int
	a := []MyInt{0,1,2}
	// b := ([]int)(a) // error: cannot convert a (type []MyInt) to type []int
	b := *(*[]int)(unsafe.Pointer(&a))
	b[0] = 3
	fmt.Println("a = ",a) // a = [3 1 2]
	fmt.Println("b = ",b) // b = [3 1 2]
	a[2] = 9 
	fmt.Println("a = ",a) // a = [3 1 9]
	fmt.Println("b = ",b) // b = [3 1 9]
}
var data *string
func Data() string{
	p := (*string)(atomic.LoadPointer((*unsafe.Pointer)(unsafe.Pointer(&data))))
	if p == nil {
		return ""
	} else {
		log.Printf("===%s %p\n",*p,p)
		return *p
	}
}
// set data atomically
func SetData(d string){
	atomic.StorePointer((*unsafe.Pointer)(unsafe.Pointer(&data)),unsafe.Pointer(&d))
}
func test_07(){
	fmt.Println("test_07")
	var wg sync.WaitGroup
	wg.Add(200) //添加200个
	for range [100] struct{}{}{
		go func(){
			time.Sleep(time.Second * time.Duration(rand.Intn(1000))/1000)
			log.Println(Data())
			wg.Done() //使用100个
		}()
	}
	for i:=range [100]struct{}{}{
		go func(i int){
			time.Sleep(time.Second * time.Duration(rand.Intn(1000))/1000)
			s := fmt.Sprint("#",i)
			log.Printf("===set %s %p\n",s,&s)
			SetData(s)
			wg.Done() //使用100个
		}(i)
	}
	wg.Wait()
	fmt.Println("final data = ",*data)
}
func main(){
	test_01()
	test_02()
	test_03()
	test_04()
	test_05()
	test_06()
	test_07()
}
