package main

import (
	"net/http"
)

func main() {
	test_01()
}
func test_01() {
	http.Handle("/", http.FileServer(http.Dir("/")))
	//http.Handle("/gospace", http.StripPrefix("/gospace", http.FileServer(http.Dir("/gospace"))))
	http.ListenAndServe(":80", nil)
}
