package sudoku

import (
	"fmt"
)

type Sudomap [][]byte

var CanChoose []map[byte]byte // 0-8 行 9-17 列 18-26 块 可填值的map
var count = 0

func (smap *Sudomap) Print() {
	for _, line := range *smap {
		for _, v := range line {
			fmt.Printf("%d ", v)
		}
		fmt.Println()
	}
}
func PrintChoose() {
	i := 0
	for _, v := range CanChoose {
		fmt.Println(i, v)
		i++
	}
}

/*
	统计空缺的个数
*/
func (smap *Sudomap) countZero() {
	for _, line := range *smap {
		for _, v := range line {
			if v == 0 { // 值为0时表示可填
				count++
			}
		}
	}
}
func makemap() (m map[byte]byte) {
	var i byte
	m = make(map[byte]byte)
	for i = 1; i <= 9; i++ {
		m[i] = i
	}
	return
}

// 初始化可以填的数的列表，为map[int]int的数组
func (smap *Sudomap) initCanChoose() {
	// 数组初始化时是27个map 每个map中的键值为1-9
	for i := 0; i < 27; i++ {
		CanChoose = append(CanChoose, makemap())
	}
	// 根据传进来的数独数据进行删除可以填写的map表
	for i := 0; i < 9; i++ {
		for j := 0; j < 9; j++ {
			if c := (*smap)[i][j]; c != 0 {
				//fmt.Println(c,i,j)
				delete(CanChoose[i], c)            // 第i行
				delete(CanChoose[j+9], c)          // 第j列
				delete(CanChoose[j/3+i/3*3+18], c) // 第？块
				//PrintChoose()
			}
		}
	}
}

/*
	判断是否可以填写，根据行，列，块map确定唯一可以填写的值
*/
func (smap *Sudomap) isCanChoose(i, j int) (byte, bool) {
	flag := 0
	var p byte
	var ok bool
	for _, v := range CanChoose[i] {
		p, ok = CanChoose[j+9][v]
		if ok != true {
			continue
		}
		p, ok = CanChoose[j/3+i/3*3+18][v]
		if ok != true {
			continue
		}
		flag++
	}
	if flag == 1 {
		//fmt.Println("isCanChoose ",p)
		return p, true
	} else {
		return p, false
	}
}

/*
	进行填值操作
*/
func (smap *Sudomap) do() {
	for i := 0; i < 9; i++ {
		for j := 0; j < 9; j++ {
			if c := (*smap)[i][j]; c == 0 {
				v, ok := smap.isCanChoose(i, j)
				if ok != true || v == 0 {
					continue
				}
				count--
				delete(CanChoose[i], v)            // 第i行
				delete(CanChoose[j+9], v)          // 第j列
				delete(CanChoose[j/3+i/3*3+18], v) // 第？块
				(*smap)[i][j] = v
				fmt.Println("我认为：（", i+1, "行", j+1, "列）为", v)
			}
		}
	}
}

/*
	主入口
*/
func (smap *Sudomap) SolveSudoku() {
	smap.countZero()
	//fmt.Println(count)
	//smap.Print()
	smap.initCanChoose()
	//PrintChoose()
	for count > 0 {
		smap.do()
	}
	//smap.Print()
}
