//+build osinfo

package main

import (
	"../html"
	"fmt"
	"net/http"
	"os"
)

func Get_plugin_name() (pluginname string) {
	pluginname = "/osinfo"
	return
}
func IsDisplay() bool {
	return true
}
func Func_plugin(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "<html><body>")
	html.Title(w, "os info")
	fmt.Fprintf(w, "<h1>info</h1>")
	fmt.Fprintf(w, "<h2>pwd:%s<h2>", os.Getenv("PWD"))
	fmt.Fprintf(w, "</body></html>")
	return
}
