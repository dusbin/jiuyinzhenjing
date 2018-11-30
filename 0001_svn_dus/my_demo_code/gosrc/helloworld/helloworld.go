package main
import ("fmt" /*使用格式化的IO*/
	"unicode/utf8"
	log "./logrus" //当前路径下的logrus目录
	)
func main(){
	fmt.Printf("hello,world or 你好，世界\n");
	var a int
	var b int
	a = 15
	b = a + a
	b = b + 5
	fmt.Printf("b is %d, a is %d。\n",b,a);
	/*goto 实现一个循环*/
	fmt.Printf("使用goto实现循环\n");
	var i int
	i = 0
Here:
	fmt.Printf("i is %d\n",i);
	i++
	if i >= 10 {
		
	}else{
		goto Here;
	}
	fmt.Printf("使用for实现循环\n");
	for i = 0; i < 10; i++{ //for ;; 死循环
		fmt.Printf("i is %d\n",i);
	}
	var str string
	str = "duzhengbin is working for uxsino Xi'an.杜争斌正在西安优炫工作"
	fmt.Printf("String %s\nLength:%d,Runes:%d\n",str,len([]byte(str)),utf8.RuneCount([]byte(str)))
	log.Info("log---Info")
	log.Debug("log---Debug")
	log.Error("log---Error")
}
