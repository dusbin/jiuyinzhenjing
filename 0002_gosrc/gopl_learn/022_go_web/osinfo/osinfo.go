//+build osinfo

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
	pluginname = "/osinfo"
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
		1. 获取当前的路径
*/
func Func_plugin(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "<html><body>")
	html.Title(w, "os info")
	fmt.Fprintf(w, "<h1>info</h1>")
	fmt.Fprintf(w, "<h2>pwd:%s<h2>", os.Getenv("PWD"))
	fmt.Fprintf(w, "</body></html>")
	return
}
