//+build wsclient

package main
import (
	//"os"
	"fmt"
	"net/http"
	"net"
	"strings"
	//"../html"
)
func Get_plugin_name()(pluginname string){
	pluginname = "/wsclient"
	return
}
func IsDisplay() bool{
	return true
}
func Func_plugin(w http.ResponseWriter,r *http.Request){
	data := "<!DOCTYPE html>\n"+
"	<html>\n" +
"	<head>\n" +
"		<meta charset=\"utf-8\">\n" +
"		<script>\n" +
"			window.addEventListener(\"load\", function(evt) {\n" +
"				var output = document.getElementById(\"output\");\n" +
"				var input = document.getElementById(\"input\");\n" +
"				var ws;\n" +
"				var print = function(message) {\n" +
"					var d = document.createElement(\"div\");\n" +
"					d.innerHTML = message;\n" +
"					output.appendChild(d);\n" +
"				};\n" +
"				document.getElementById(\"open\").onclick = function(evt) {\n" +
"				if (ws) {\n" +
"						return false;\n" +
"					}\n" +
"					ws = new WebSocket(\"ws://%s:80/wsserver\");\n" +
"					ws.onopen = function(evt) {\n" +
"						print(\"OPEN\");\n" +
"					}\n" +
"					ws.onclose = function(evt) {\n" +
"						print(\"CLOSE\");\n" +
"						ws = null;\n" +
"					}\n" +
"					ws.onmessage = function(evt) {\n" +
"						print(\"RESPONSE: \" + evt.data);\n" +
"					}\n" +
"					ws.onerror = function(evt) {\n" +
"						print(\"ERROR: \" + evt.data);\n" +
"					}\n" +
"					return false;\n" +
"				};\n" +
"				document.getElementById(\"send\").onclick = function(evt) {\n" +
"					if (!ws) {\n" +
"						return false;\n" +
"					}\n" +
"					print(\"SEND: \" + input.value);\n" +
"					ws.send(input.value);\n" +
"					return false;\n" +
"				};\n" +
"				document.getElementById(\"close\").onclick = function(evt) {\n" +
"					if (!ws) {\n" +
"						return false;\n" +
"					}\n" +
"					ws.close();\n" +
"					return false;\n" +
"				};\n" +
"			});\n" +
"		</script>\n" +
"	</head>\n" +
"	<body>\n" +
"	<table>\n" +
"		<tr><td valign=\"top\" width=\"50%%%%\">\n" +
"			<p>Click \"Open\" to create a connection to the server,\n" +
"			\"Send\" to send a message to the server and \"Close\" to close the connection.\n" +
"			You can change the message and send multiple times.\n" +
"			</p>\n" +
"			<form>\n" +
"				<button id=\"open\">Open</button>\n" +
"				<button id=\"close\">Close</button>\n" +
"				<input id=\"input\" type=\"text\" value=\"Hello world!\">\n" +
"				<button id=\"send\">Send</button>\n" +
"			</form>\n" +
"		</td><td valign=\"top\" width=\"50%%%%\">\n" +
"		<div id=\"output\"></div>\n" +
"		</td></tr></table>\n" +
"	</body>\n" +
"	</html>\n"
	serverip := findServerIpFromIpAddr(r.RemoteAddr)
	var data1 string
	fmt.Println("serverip:",serverip)
	data1 = fmt.Sprintf(data,serverip)
	fmt.Fprintf(w,data1)
	//html.Title(w,"user info")
	//fmt.Fprintf(w,"<h1>info</h1>")
	//fmt.Fprintf(w,"<h2>user:%s<h2>",os.Getenv("USER"))
	//fmt.Fprintf(w,"</body></html>")
	return
}
// 查找客户端从哪个网卡访问的，返回对应的ip
func findServerIpFromIpAddr(clientip string) (serverip string){
	// 过滤掉端口信息
	kv := strings.Split(clientip,":")
	clientip = kv[0]
	// 将string的ip转换为IP类型的ip
	ip := net.ParseIP(clientip)
	addrs,err := net.InterfaceAddrs()
	if err != nil {
		serverip = "127.0.0.1"
		return
	}
	for _,address := range addrs {
		ipnet := address.(*net.IPNet)
		if ipnet.IP.To4() != nil {
			if ok := ipnet.Contains(ip);ok == true {
				serverip = ipnet.IP.String()
				return
			}
		}
	}
	serverip = "127.0.0.1"
	return
}