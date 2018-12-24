//+build wsclient

package main

import (
	//"os"
	"fmt"
	"net"
	"net/http"
	"strings"
	//"../html"
)

/*
	获取插件名称
*/
func Get_plugin_name() (pluginname string) {
	pluginname = "/wsclient"
	return
}

/*
	插件是否展示
*/
func IsDisplay() bool {
	return true
}

/*
	执行插件的功能
		1. 生成一个client的客户端网页
*/
func Func_plugin(w http.ResponseWriter, r *http.Request) {
	data := `
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<script>
	window.addEventListener("load", function(evt) {
		var output = document.getElementById("output");
		var input = document.getElementById("input");
		var ws;
		var print = function(message) {
			var d = document.createElement("div");
			d.innerHTML = message;
			output.appendChild(d);
		};
		document.getElementById("open").onclick = function(evt) {
			if (ws) {
				return false;
			}
			ws = new WebSocket("ws://%s:80/wsserver");
			ws.onopen = function(evt) {
				print("OPEN");
			}
			ws.onclose = function(evt) {
				print("CLOSE");
				ws = null;
			}
			ws.onmessage = function(evt) {
				print("RESPONSE: " + evt.data);
			}
			ws.onerror = function(evt) {
				print("ERROR: " + evt.data);
			}
			return false;
		};
		document.getElementById("send").onclick = function(evt) {
			if (!ws) {
				return false;
			}
			print("SEND: " + input.value);
			ws.send(input.value);
			return false;
		};
		document.getElementById("close").onclick = function(evt) {
			if (!ws) {
				return false;
			}
			ws.close();
			return false;
		};
	});
</script>
</head>
<body>
<table>
	<tr>
		<td valign="top" width="50%%">
			<p>Click "Open" to create a connection to the server,
				"Send" to send a message to the server and "Close" to close the connection.
				You can change the message and send multiple times.
			</p>
			<form>
				<button id="open">Open</button>
				<button id="close">Close</button>
				<input id="input" type="text" value="Hello world!">
				<button id="send">Send</button>
			</form>
		</td>
		<td valign="top" width="50%%">
			<div id="output"></div>
		</td>
	</tr>
</table>
</body>
</html>`
	serverip := findServerIpFromIpAddr(r.RemoteAddr)
	var data1 string
	fmt.Println("serverip:", serverip)
	data1 = fmt.Sprintf(data, serverip)
	fmt.Fprintf(w, data1)
	return
}

// 查找客户端从哪个网卡访问的，返回对应的ip
func findServerIpFromIpAddr(clientip string) (serverip string) {
	// 过滤掉端口信息
	kv := strings.Split(clientip, ":")
	clientip = kv[0]
	// 将string的ip转换为IP类型的ip
	ip := net.ParseIP(clientip)
	addrs, err := net.InterfaceAddrs()
	if err != nil {
		serverip = "127.0.0.1"
		return
	}
	for _, address := range addrs {
		ipnet := address.(*net.IPNet)
		if ipnet.IP.To4() != nil {
			if ok := ipnet.Contains(ip); ok == true {
				serverip = ipnet.IP.String()
				return
			}
		}
	}
	serverip = "127.0.0.1"
	return
}
