package main
import(
	"fmt"
	//"os"
	"time"
	"math/rand"
)
func main(){
	//获取时间
	fmt.Println("time.Now().UnixNano():[",time.Now().UnixNano(),"]")
	//使用时间生成随机数种子
	rand.Seed(time.Now().UnixNano())
	//生成随机数
	fmt.Println("rand.Intn(100):[",rand.Intn(100),"]")
	fmt.Println("rand.Intn(100):[",rand.Intn(100),"]")
	fmt.Println("rand.Intn(100):[",rand.Intn(100),"]")
	fmt.Println("rand.Intn(100):[",rand.Intn(100),"]")
	fmt.Println("rand.Intn(100):[",rand.Intn(100),"]")
}