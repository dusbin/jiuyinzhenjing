package main

import (
	"./server"
	"os"
	"os/exec"
	"path/filepath"
	"time"
)

/*
	程序的主入口
	第一部分实现后台守护进程
	第二部分在go routine中实现server的运行
*/
func main() {
	if os.Getppid() != 1 {
		filePath, _ := filepath.Abs(os.Args[0])
		cmd := exec.Command(filePath, os.Args[1:]...)
		//cmd.Stdin = os.Stdin
		//cmd.Stdout = os.Stdout
		//cmd.Stderr = os.Stderr
		cmd.Start()
		return
	}
	for {
		if server.Running == 0 {
			go server.Server()
		}
		time.Sleep(time.Second)
	}
}
