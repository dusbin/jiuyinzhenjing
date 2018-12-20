package main

import (
	"fmt"
	"strconv"
	"sync"
)

func main() {
	//test_01()
	//test_02() // 使用读写锁实现并发安全的map
	test_03() // sync.Map 实现
}

/* sync.Map原型
type Map struct{
	mu Mutex
	read atomic.Value
	dirty map[interface{}]*entry
	misses int
}
*/
func test_03() {
	var m sync.Map
	m.Store("method", "eth_getBlockByHash")
	m.Store("jsonrpc", "2.0")
	m.Range(func(key, value interface{}) bool {
		fmt.Println(key, value)
		return true
	})
}

type M struct {
	Map  map[string]string
	lock sync.RWMutex // 锁
}

func (m *M) Set(key, value string) {
	m.lock.Lock()
	m.Map[key] = value
	m.lock.Unlock()
}
func (m *M) Get(key string) string {
	return m.Map[key]
}

// 存在 map读写错误，并发错误
/*
fatal error: concurrent map writes
5 5

goroutine 13 [running]:
runtime.throw(0x4b5eb5, 0x15)
	/usr/local/go/src/runtime/panic.go:616 +0x81 fp=0xc42002deb8 sp=0xc42002de98 pc=0x4267f1
runtime.mapassign_faststr(0x49af00, 0xc420066150, 0x4ba68b, 0x1, 0x0)
	/usr/local/go/src/runtime/hashmap_fast.go:703 +0x3e9 fp=0xc42002df28 sp=0xc42002deb8 pc=0x40a0c9
main.(*M).Set(...)

*/
func test_02() {
	c := M{Map: make(map[string]string)}
	wg := sync.WaitGroup{}
	for i := 0; i < 21; i++ {
		wg.Add(1)
		go func(n int) {
			k, v := strconv.Itoa(n), strconv.Itoa(n)
			c.Set(k, v)
			fmt.Println(k, c.Get(k))
			wg.Done()
		}(i)
	}
	wg.Wait()
}
func test_01() {
	scene := make(map[string]int)
	scene["route"] = 66
	scene["brazil"] = 4
	scene["china"] = 960
	for k, v := range scene {
		fmt.Println(k, v)
	}
	for k, _ := range scene {
		delete(scene, k)
		for k, v := range scene {
			fmt.Println(k, v)
		}
	}
}
