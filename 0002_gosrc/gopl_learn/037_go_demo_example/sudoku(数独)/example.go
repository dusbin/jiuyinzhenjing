package main
import (
	"./sudoku"
)
func main(){
	//var smap sudoku.Sudomap
	//smap = make([]byte,9)
	//for i:= 0; i<len(smap);i++{
	//	smap[i] = make (byte,9) 
	//}
	smap := sudoku.Sudomap{
		{5,3,0,0,7,0,0,0,0},
		{6,0,0,1,9,5,0,0,0},
		{0,9,8,0,0,0,0,6,0},
		{8,0,0,0,6,0,0,0,3},
		{4,0,0,8,0,3,0,0,1},
		{7,0,0,0,2,0,0,0,6},
		{0,6,0,0,0,0,2,8,0},
		{0,0,0,4,1,9,0,0,5},
		{0,0,0,0,8,0,0,7,9},
	}
	smap.SolveSudoku()
}