package main
import (
	"fmt"
	"log"
	"time"
	"github.com/radovskyb/watcher"
)
func main(){
	w:=watcher.New()
	w.SetMaxEvents(1)
	w.FilterOps(watcher.Rename,watcher.Move,watcher.Create,watcher.Remove,watcher.Chmod,watcher.Write)
	go func(){
		for {
			select {
				case event := <-w.Event :
					fmt.Println(event)
				case err := <-w.Error:
					log.Fatalln(err)
				case <-w.Closed:
					return
			}
		}
	}()
	if err:= w.Add(".");err != nil {
		log.Fatalln(err)
	}
	if err := w.AddRecursive("/test_folder");err != nil {
		log.Fatalln(err)
	}
	for path,f:= range w.WatchedFiles(){
		fmt.Println(path," : ",f.Name())
	}
	fmt.Println()
	go func(){
		w.Wait()
		w.TriggerEvent(watcher.Create,nil)
		w.TriggerEvent(watcher.Remove,nil)
	}()
	if err:=w.Start(time.Millisecond *100);err!=nil{
		log.Fatalln(err)
	}
	//for{
	//	time.Sleep(time.Minute*5)
	//}
}