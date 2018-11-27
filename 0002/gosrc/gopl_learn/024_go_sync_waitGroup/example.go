package main
import (
	"fmt"
	"time"
	"os"
	"sync"
	"math/rand"
)
var waitGroup = new(sync.WaitGroup)
func work(v int){
	fmt.Println("v is:",v)
	for j := 0;j < v;j++{
		time.Sleep(time.Second)
	}
	fmt.Println("down v:",v)
	waitGroup.Done()
}
func main(){
	os.MkdirAll("pic2016",0666)
	now := time.Now()
	for i:=1; i<24; i++{
		waitGroup.Add(1)
		v := rand.Int()%32
		go work(v)
	}
	waitGroup.Wait()
	fmt.Println("end time:",time.Now().Sub(now))
}