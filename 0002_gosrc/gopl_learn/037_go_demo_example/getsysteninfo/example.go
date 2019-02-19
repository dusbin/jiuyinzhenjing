package main

// https://cloud.tencent.com/developer/article/1072680
import (
	"./gopsutil/cpu"
	"./gopsutil/mem"
	"fmt"
	"time"
	//"github.com/shirou/gopsutil/mem"
)

func main() {
	v, _ := mem.VirtualMemory()
	c, _ := cpu.Info()
	cc, _ := cpu.Percent(time.Second, false)

	fmt.Println("memory   :", v)
	fmt.Println("cpu 1    :", c)
	fmt.Println("cpu2     :", cc)
}
