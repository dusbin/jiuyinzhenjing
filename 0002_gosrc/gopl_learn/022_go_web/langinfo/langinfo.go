//+build lang info

package main

import (
	"../html"
	"fmt"
	"net/http"
	"os"
)

func Get_plugin_name() (pluginname string) {
	pluginname = "/langinfo"
	return
}
func IsDisplay() bool {
	return true
}
func Func_plugin(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "<html><body>")
	html.Title(w, "lang info")
	fmt.Fprintf(w, "<h1>info</h1>")
	fmt.Fprintf(w, "<h2>lang:%s<h2>", os.Getenv("LANG"))
	fmt.Fprintf(w, "</body></html>")
	return
}
