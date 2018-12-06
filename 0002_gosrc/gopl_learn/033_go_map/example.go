package main
import (
	"fmt"
)
func main(){
	scene := make(map[string]int)
	scene["route"] = 66
	scene["brazil"] = 4
	scene["china"] = 960
	for k,v:= range scene {
		fmt.Println(k,v)
	} 
	for k,_:= range scene {
		delete(scene,k)
		for k,v:= range scene {
			fmt.Println(k,v)
		}
	}
}