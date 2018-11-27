package server
import (
	"net"
	"strings"
	"net/http"
	"fmt"
	"../html"
	"time"
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
	port:=":80"
	sport:=":8080"
	for _,v:=range dir_list{
		if strings.HasSuffix(v.Name(),".so") == true{
			plugin_file:="./"+v.Name()
			pdll,err:= plugin.Open(plugin_file)
			if err != nil{
				panic(err)
			}
			get_plugin_name_func,err:=pdll.Lookup("Get_plugin_name")
			if err != nil{
				panic(err)
			}
			plugin_name:=get_plugin_name_func.(func()(string))()
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
	addrs,err:=net.InterfaceAddrs()
	if err != nil {
		panic(err)
	}
	for _,address :=range addrs{
		ipnet:=address.(*net.IPNet)
		//if ipnet,ok:=address.(*net.IPNet);ok && !ipnet.IP.IsLoopback(){//回环地址
			if ipnet.IP.To4()!= nil {
				fmt.Println("access http://",ipnet.IP.String(),port)
			}
		//}
	}
	go func(){
		http.ListenAndServe(port,nil)//由于会阻塞，放在goroutine中，不影响下一个服务的启动，可以多开几个端口作为服务入口
	}()
	go func(){
		http.ListenAndServeTLS(sport,"cert.pem","key.pem",nil)////由于会阻塞，放在goroutine中，不影响下一个服务的启动，可以多开几个端口作为服务入口
	}()
	for {//没有常驻进程，主程序退出，goroutine也不能再存活，写一个for循环禁止main退出，也可以把最后一个服务放到main中而不是goroutine中，一般不建议那么做。
		time.Sleep(time.Minute)
	}
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