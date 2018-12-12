//+build userinfo

package main

import (
	"../html"
	"fmt"
	"net/http"
	"os"
)

/*
	获取插件名称
*/
func Get_plugin_name() (pluginname string) {
	pluginname = "/userinfo"
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
		1. 获取正在使用的user
*/
func Func_plugin(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "<html><body>")
	html.Title(w, "user info")
	fmt.Fprintf(w, "<h1>info</h1>")
	fmt.Fprintf(w, "<h2>user:%s<h2>", os.Getenv("USER"))
	fmt.Fprintf(w, "</body></html>")
	return
}
