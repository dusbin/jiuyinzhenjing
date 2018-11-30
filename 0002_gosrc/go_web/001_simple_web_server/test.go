package main
import(
	"fmt"
	"net/http"
	"strings"
	"log"
)
func main(){
	http.HandleFunc("/",sayhelloName)//设置访问路由
	err:=http.ListenAndServe(":9090",nil)
	if err!=nil{
		log.Fatal("ListenAndServe:",err)
	}
}
func sayhelloName(w http.ResponseWriter,r *http.Request){
	r.ParseForm()
	fmt.Println(r.Form)
	fmt.Println("path",r.URL.Path)
	fmt.Println("Scheme",r.URL.Scheme)
	fmt.Println(r.Form["url_long"])
	for k,v := range r.Form{
		fmt.Println("key:",k)
		fmt.Println("val:",strings.Join(v,""))
	}
	fmt.Fprintf(w,"Hello astaxie!")
}
//http://192.168.40.75:9090/?url_log=1&url_log=2
//http://192.168.40.75:9090/