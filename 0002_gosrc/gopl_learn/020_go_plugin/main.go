package main
import (
	"plugin"
)
func main(){
	for i:=0;i<10;i++{
		if i%2 == 0{
			pdll,err:= plugin.Open("./plugin1.so")
			if err != nil{
				continue
				panic(err)
			}
			funcPrint,err:=pdll.Lookup("PrintTest")
			if err != nil{
				continue
				panic(err)
			}
			funcPrint.(func(string))("Hello go plugin")
		}else {
			pdll,err:= plugin.Open("./plugin2.so")
			if err != nil{
				continue
				panic(err)
			}
			funcPrint,err:=pdll.Lookup("PrintTest")
			if err != nil{
				continue
				panic(err)
			}
			funcPrint.(func(string))("Hello go plugin")
		}	
	}
}