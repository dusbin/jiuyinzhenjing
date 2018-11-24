//+build user info

package main
import (
	"os"
	"fmt"
	"net/http"
	"../html"
)
func Get_plugin_name()(pluginname string){
	pluginname = "/userinfo"
	return
}
func Func_plugin(w http.ResponseWriter,r *http.Request){
	fmt.Fprintf(w,"<html><body>")
	html.Title(w,"user info")
	fmt.Fprintf(w,"<h1>info</h1>")
	fmt.Fprintf(w,"<h2>user:%s<h2>",os.Getenv("USER"))
	fmt.Fprintf(w,"</body></html>")
	return
}