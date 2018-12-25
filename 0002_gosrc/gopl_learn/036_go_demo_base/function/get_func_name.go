package main

import (
	"fmt"
	"runtime"
	"strconv"
	"strings"
	"time"
)

func dowork() {
	fmt.Println("goid is :", GoID())
}
func main() {
	Bar()
}
func Bar() {
	fmt.Println(runtime.GetFuncName(1), runtime.GetFuncName(2), runtime.GetFuncName(3))
	trace()
	trace2()
	DumpStacks()
	fmt.Println("goid is :", GoID())
	for i := 0; i < 5; i++ {
		go dowork()
	}
	time.Sleep(time.Second)
}

/*
	打印函数调用栈
*/
func trace2() {
	pc := make([]uintptr, 10) // at least 1 entry needed
	n := runtime.Callers(0, pc)
	frames := runtime.CallersFrames(pc[:n])
	for {
		frame, more := frames.Next()
		fmt.Printf("%s:%d %s\n", frame.File, frame.Line, frame.Function)
		if !more {
			break
		}
	}
}

/*
	打印函数调用栈
*/
func trace() {
	pc := make([]uintptr, 10) // at least 1 entry needed
	n := runtime.Callers(0, pc)
	for i := 0; i < n; i++ {
		f := runtime.FuncForPC(pc[i])
		file, line := f.FileLine(pc[i])
		fmt.Printf("%s:%d %s\n", file, line, f.Name())
	}
}
func DumpStacks() {
	buf := make([]byte, 16384)
	buf = buf[:runtime.Stack(buf, true)]
	fmt.Printf("=== BEGIN goroutine stack dump ===\n%s\n=== END goroutine stack dump ===\n", buf)
}
func GoID() int {
	var buf [64]byte
	n := runtime.Stack(buf[:], false)
	idField := strings.Fields(strings.TrimPrefix(string(buf[:n]), "goroutine "))[0]
	id, err := strconv.Atoi(idField)
	if err != nil {
		panic(fmt.Sprintf("cannot get goroutine id: %v", err))
	}
	return id
}
