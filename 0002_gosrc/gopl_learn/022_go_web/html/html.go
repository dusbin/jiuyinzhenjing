package html

import (
	"fmt"
	"net/http"
)

func Title(w http.ResponseWriter, title string) {
	fmt.Fprintf(w, "<title>%s</title>", title)
}
