package main

import (
	"fmt"
	"sync"
	"time"
)

func main() {
	var swg sync.WaitGroup
	for i:=0;i<5;i++{
		swg.Add(1)
		go func(wg *sync.WaitGroup,i int){
			//defer wg.Done()
			time.Sleep(time.Second )
			fmt.Println("Print",i)
			wg.Done()
		}(&swg,i)
	}
	fmt.Println("begin Wait")
	swg.Wait()
	fmt.Println("end Wait")
}
