//+build lang info

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
	pluginname = "/langinfo"
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
		1. 获取当前的语言信息
*/
func Func_plugin(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "<html><body>")
	html.Title(w, "lang info")
	fmt.Fprintf(w, "<h1>info</h1>")
	fmt.Fprintf(w, "<h2>lang:%s<h2>", os.Getenv("LANG"))
	fmt.Fprintf(w, "</body></html>")
	return
}
