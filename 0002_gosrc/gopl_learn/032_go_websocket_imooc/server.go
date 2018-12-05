package main

import (
	"time"
	"net/http"
	"github.com/gorilla/websocket"
	"./impl"
)

var (
	// 定义一个转换器
	upgrader = websocket.Upgrader{
		// 握手时允许跨域 websocket服务端一般独立部署在一个域名下的，
		// 从浏览器发起一个连接到websocket服务器时,是一个跨域的
		CheckOrigin:func(r *http.Request)bool {
			return true/*return true 允许跨域*/
		},	
	}   
)
/*
func wsHandler(w http.ResponseWriter,r *http.Request){
	var (
		conn *websocket.Conn
		err error
		data []byte
	)
	// Upgrade:websocket
	if conn,err = upgrader.Upgrade(w,r,nil);err != nil {
		return
	}
	// websocket.Conn
	for {
		// Message支持的类型 Text Binary
		if _,data,err = conn.ReadMessage();err != nil {
			goto ERR
		}
		if err = conn.WriteMessage(websocket.TextMessage,data);err != nil {
			goto ERR
		}
	}
ERR:
	conn.Close()
}
*/
func wsHandler(w http.ResponseWriter,r *http.Request){
	var (
		wsConn *websocket.Conn
		err error
		data []byte
		conn *impl.Connection
	)
	// Upgrade:websocket
	if wsConn,err = upgrader.Upgrade(w,r,nil);err != nil {
		return
	}
	if conn,err = impl.InitConnection(wsConn);err != nil {
		goto ERR
	}
	// 心跳消息
	go func(){
		var err error
		for {
			if err = conn.WriteMessage([]byte("heartbeat"));err != nil{
				return
			}
			time.Sleep(1*time.Second)
		}
	}()
	for {
		if data,err = conn.ReadMessage();err != nil {
			goto ERR
		}
		if err = conn.WriteMessage(data);err != nil {
			goto ERR
		}
	}
ERR:
	// TODO:关闭连接的操作
	conn.Close()
}
func main(){
	http.HandleFunc("/ws",wsHandler)
	http.ListenAndServe(":7777",nil)
}