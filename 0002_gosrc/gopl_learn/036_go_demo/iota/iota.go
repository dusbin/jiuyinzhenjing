package main

import (
	"fmt"
)

/*
	每增加一次，iota计数增加1
	const 遇到非整数，没有值的话重复上一行的
	中间有插队的
*/
const ( // 每一次const出现时，都会让iota初始化为0 自增长
	y = 10   // 10
	z        // 10
	a = iota // 2	从第一个开始的计数，为2
	b        // 3
	c        // 4
	d = "ha" // ha
	e        // ha
	f = 100  // 100
	g        // 100
	h = iota // 9	// 恢复了计数
	i        // 10
)

func test_01() {
	fmt.Println(y, z, a, b, c, d, e, f, g, h, i)
}

/*
	自定义类型
*/
type Weekday int

const (
	Sunday    Weekday = iota // 0
	Monday                   // 1
	Tuesday                  // 2
	Wednesday                // 3
	Thursday                 // 4
	Friday                   // 5
	Saturday                 // 6
)

/*
	可跳过值
*/
type AudioOutput int

const (
	OutMute   AudioOutput = iota // 0
	OutMono                      // 1
	OutStereo                    // 2
	_
	_
	OutSurround // 5
)

/*
	位掩码表达式 可应用于开关，IgEggs|IgChocolate|IgShellfish 00010011
*/
type Allergen int

const (
	IgEggs         Allergen = 1 << iota // 1 << 0 which is 00000001
	IgChocolate                         // 1 << 1 which is 00000010
	IgNuts                              // 1 << 2 which is 00000100
	IgStrawberries                      // 1 << 3 which is 00001000
	IgShellfish                         // 1 << 4 which is 00010000
)

type ByteSize float64

const (
	_           = iota             // ignore first value by assigning to blank identifier
	KB ByteSize = 1 << (10 * iota) // 1 << (10*1)
	MB                             // 1 << (10*2)
	GB                             // 1 << (10*3)
	TB                             // 1 << (10*4)
	PB                             // 1 << (10*5)
	EB                             // 1 << (10*6)
	ZB                             // 1 << (10*7)
	YB                             // 1 << (10*8)
)
const (
	_   = 1 << (10 * iota)
	KiB // 1,024
	MiB // 1,048,576
	GiB // 1,073,741,824
	TiB // 1,099,511,627,778				(exceeds 1 << 32)
	PiB // 1,125,899,906,842,624
	EiB // 1,152,921,504,606,846,976
	ZiB // 1,180,591,620,717,411,303,424	(exceeds 1 << 64)
	YiB // 1,208,925,819,614,629,174,706,176
)

/*
	定义在一行的情况
*/
const (
	Apple, Banana     = iota + 1, iota + 2 // 1 2
	Cherimoya, Durian                      // 2 3
	Elderberry, Fig                        // 3 4
)

func main() {
	// fmt.Println(iota)// iota只能用于常量表达式，不能直接使用
	test_01()
}
