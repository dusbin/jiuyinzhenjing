package server
import (
	"os"
	"html/template"
	"encoding/hex"
	"crypto/md5"
	"net"
	"strings"
	"net/http"
	"fmt"
	"../html"
	"time"
	"plugin"
	"io/ioutil"
	"io"
	"strconv"
)
var count = 0
var Running = 0
var plugin_list = []string{}
func Server(){
	dir_list,error:=ioutil.ReadDir("./")
	if error!= nil{
		panic(error)
	}
	port:=":80"
	sport:=":8080"
	plugin_list = []string{}
	for _,v:=range dir_list{
		if strings.HasSuffix(v.Name(),".so") == true{
			plugin_file:="./"+v.Name()
			pdll,err:= plugin.Open(plugin_file)
			if err != nil{
				fmt.Println(err)
				continue
			}
			get_plugin_name_func,err:=pdll.Lookup("Get_plugin_name")
			if err != nil{
				fmt.Println(err)
				continue
			}
			plugin_name:=get_plugin_name_func.(func()(string))()
			plugin_func,err:=pdll.Lookup("Func_plugin")
			if err != nil{
				fmt.Println(err)
				continue
			}
			plg_func:=plugin_func.(func(http.ResponseWriter,*http.Request)())
			http.HandleFunc(plugin_name,plg_func)
			display_func,err :=pdll.Lookup("IsDisplay")
			if err != nil{
				fmt.Println(err)
				continue
			}
			isDisplay := display_func.(func()(bool))()
			if isDisplay {
				plugin_list = append(plugin_list,plugin_name)
			}
		}
	}
	http.HandleFunc("/",index)
	http.HandleFunc("/upload",upload)
	http.HandleFunc("/uploadfile",uploadfile)
	http.HandleFunc("/restart",restart)
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
		fmt.Println("Start http")
		http.ListenAndServe(port,nil)//由于会阻塞，放在goroutine中，不影响下一个服务的启动，可以多开几个端口作为服务入口
	}()
	go func(){
		fmt.Println("Start https")
		http.ListenAndServeTLS(sport,"cert.pem","key.pem",nil)////由于会阻塞，放在goroutine中，不影响下一个服务的启动，可以多开几个端口作为服务入口
	}()
	Running = 1
	for {//没有常驻进程，主程序退出，goroutine也不能再存活，写一个for循环禁止main退出，也可以把最后一个服务放到main中而不是goroutine中，一般不建议那么做。
		if Running == 1 {
			//time.Sleep(time.Minute)
			time.Sleep(time.Second)
		}else {
			fmt.Println("restart")
			// 自己实现的一个清扫 defaultServeMux.m （type ServeMux）的方法
			// 主要解决当插件不存在时，注册函数依然存在，还可以访问不存在的插件功能
			http.CleanServeMuxMap()
			break
		}
	}
}
func upload(w http.ResponseWriter,r *http.Request){
	fmt.Fprintf(w,"<html><body>")
	html.Title(w,"upload")
	fmt.Fprintf(w,"<form enctype=\"multipart/form-data\" action=\"/uploadfile\" method=\"post\"> <input type=\"file\" name=\"uploadfile\" /> <input type=\"hidden\" name=\"token\" value=\"{{.}}\"/> <input type=\"submit\" value=\"upload\" /> </form>")
	fmt.Fprintf(w,"</body></html>")
}
func restart(w http.ResponseWriter,r *http.Request){
	Running = 0
	fmt.Fprintf(w,"<html><body>")
	html.Title(w,"restart")
	fmt.Fprintf(w,"<h2>restart ok</h2>")
	fmt.Fprintf(w,"<h2><a href=\"/index\">back to home</a></h2>")
	fmt.Fprintf(w,"</body></html>")
}
func uploadfile(w http.ResponseWriter,r *http.Request){
	r.ParseForm()
	if r.Method == "GET"{ 
		time := time.Now().Unix() 
		h := md5.New() 
		h.Write([]byte(strconv.FormatInt(time,10))) 
		token := hex.EncodeToString(h.Sum(nil)) 
		t, _ := template.ParseFiles("./view/upload.ctpl") 
		t.Execute(w, token) 
	}else if r.Method == "POST"{ 
		//把上传的文件存储在内存和临时文件中 
		r.ParseMultipartForm(32 << 20) 
		//获取文件句柄，然后对文件进行存储等处理 
		file, handler, err := r.FormFile("uploadfile")
		fmt.Fprintf(w,"<html><body>")
		html.Title(w,"upload resault")
		if err != nil{ 
			fmt.Fprintf(w,"<h2>update[%s] faile</h2>",handler.Filename)
			fmt.Fprintf(w,"</body></html>")
			return 
		} 
		defer file.Close() 
		//fmt.Fprintf(w, "%v", handler.Header) 
		//创建上传的目的文件 
		f, err := os.OpenFile("./" + handler.Filename, os.O_WRONLY | os.O_CREATE, 0766) 
		if err != nil{ 
			fmt.Fprintf(w,"<h2>update[%s] faile</h2>",handler.Filename)
			fmt.Fprintf(w,"</body></html>")
			return 
		} 
		defer f.Close() 
		//拷贝文件 
		io.Copy(f, file) 
		fmt.Fprintf(w,"<h2>update[%s] ok</h2>",handler.Filename)
		fmt.Fprintf(w,"<h2><a href=\"/restart\">restart</a></h2>")
		fmt.Fprintf(w,"</body></html>")
	}
}
func index(w http.ResponseWriter,r *http.Request){
	fmt.Fprintf(w,"<html><body>")
	html.Title(w,"main")
	fmt.Fprintf(w,"<h1>Features List</h1>")
	for _,v:=range plugin_list{
		fmt.Fprintf(w,"<h2><a href=\"%s\">%s</a></h2>",v,v)
	}
	fmt.Fprintf(w,"<h2><a href=\"/upload\">upload plugin</a></h2>")
	fmt.Fprintf(w,"<h2><a href=\"/restart\">restart</a></h2>")
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