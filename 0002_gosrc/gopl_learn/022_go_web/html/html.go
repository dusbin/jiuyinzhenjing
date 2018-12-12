package html

import (
	"fmt"
	"net/http"
)

/*
	title标签
*/
func Title(w http.ResponseWriter, title string) {
	fmt.Fprintf(w, "<title>%s</title>", title)
}
