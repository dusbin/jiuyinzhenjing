package server
import (
	"strings"
	"net/http"
	"fmt"
	"../html"
	//"time"
	"plugin"
	"io/ioutil"
)
var count = 0
var plugin_list = []string{}
func Server(){
	dir_list,error:=ioutil.ReadDir("./")
	if error!= nil{
		panic(error)
	}
	//plugin_list:=[]string{}
	for _,v:=range dir_list{
		if strings.HasSuffix(v.Name(),".so") == true{
			fmt.Println(v.Name())
			plugin_file:="./"+v.Name()
			fmt.Println(plugin_file)
			pdll,err:= plugin.Open(plugin_file)
			if err != nil{
				panic(err)
			}
			get_plugin_name_func,err:=pdll.Lookup("Get_plugin_name")
			if err != nil{
				panic(err)
			}
			plugin_name:=get_plugin_name_func.(func()(string))()
			fmt.Println("plugin name:",plugin_name)
			plugin_func,err:=pdll.Lookup("Func_plugin")
			if err != nil{
				panic(err)
			}
			plg_func:=plugin_func.(func(http.ResponseWriter,*http.Request)())
			http.HandleFunc(plugin_name,plg_func)
			plugin_list = append(plugin_list,plugin_name)
		}
	}
	http.HandleFunc("/",index)
	http.ListenAndServe(":8000",nil)
}
func index(w http.ResponseWriter,r *http.Request){
	fmt.Fprintf(w,"<html><body>")
	html.Title(w,"main")
	fmt.Fprintf(w,"<h1>Features List</h1>")
	for _,v:=range plugin_list{
		fmt.Fprintf(w,"<h2><a href=\"%s\">%s</a></h2>",v,v)
	}
	/*
	flag:=1
	if r.FormValue("name") != ""{
		fmt.Fprintf(w,"<h2>name : %s</h2>",r.FormValue("name"))
		flag = 0
	}
	if r.FormValue("tel") != ""{
		fmt.Fprintf(w,"<h2>tel : %s</h2>",r.FormValue("tel"))
		flag = 0
	}
	if flag == 1{
		fmt.Fprintf(w,"<h2>info is empty</h2>")
	}
	*/
	fmt.Fprintf(w,"</body></html>")
	return
}