package impl

import (
	"errors"
	"fmt"
	"github.com/gorilla/websocket"
	"sync"
)

/*
	定义websocket管理的结构体
*/
type Connection struct {
	wsConn    *websocket.Conn
	inChan    chan []byte
	outChan   chan []byte
	closeChan chan byte
	isClosed  bool
	mutex     sync.Mutex // isClosed 需要加锁
}

/*
	初始化连接
*/
func InitConnection(wsConn *websocket.Conn) (conn *Connection, err error) {
	conn = &Connection{
		wsConn:    wsConn,
		inChan:    make(chan []byte, 1000),
		outChan:   make(chan []byte, 1000),
		closeChan: make(chan byte, 1), // 作用在于通知退出
	}
	// 启动读协程
	go conn.ReadLoop()
	// 启动写协程
	go conn.WriteLoop()
	return
}

// API
/*
	从client读取消息
*/
func (conn *Connection) ReadMessage() (data []byte, err error) {
	select {
	case data = <-conn.inChan:
	case <-conn.closeChan:
		err = errors.New("connection is closed")
	}
	// data = <- conn.inChan
	return
}

/*
	向client写消息
*/
func (conn *Connection) WriteMessage(data []byte) (err error) {
	select {
	case conn.outChan <- data:
	case <-conn.closeChan:
		err = errors.New("connection is closed")
	}
	// conn.outChan <- data
	return
}

/*
	线程安全的关闭连接
*/
func (conn *Connection) Close() {
	// 线程安全的，可以并发调用，也可以多次调用（可重入的）
	fmt.Println("close")
	conn.wsConn.Close()
	//close(conn.closeChan)可能多次调用，不是可重入的，要保证只执行一次
	conn.mutex.Lock()
	if !conn.isClosed {
		close(conn.closeChan) // 创造ReadLoop中select的closeChan消息执行goto ERR
		conn.isClosed = true
	}
	conn.mutex.Unlock()
	return
}

// 内部实现
/*
	读循环
*/
func (conn *Connection) ReadLoop() {
	var (
		data []byte
		err  error
	)
	for {
		if _, data, err = conn.wsConn.ReadMessage(); err != nil {
			goto ERR
		}
		// inChan 满了会被阻塞
		// conn.inChan <- data
		// 阻塞结束后 WriteLoop 已经关闭了websocket，会一直被阻塞 增加 close chan
		// 如下解决使用
		select {
		case conn.inChan <- data:
		case <-conn.closeChan:
			goto ERR
		}
	}
ERR:
	conn.Close()
}

/*
	写循环
*/
func (conn *Connection) WriteLoop() {
	var (
		data []byte
		err  error
	)
	for {
		//data = <-conn.outChan //如果inChan阻塞，会退出循环，关闭webSocket
		select {
		case data = <-conn.outChan:
		case <-conn.closeChan:
			goto ERR
		}
		if err = conn.wsConn.WriteMessage(websocket.TextMessage, data); err != nil {
			goto ERR
		}
	}
ERR:
	conn.Close()
}
