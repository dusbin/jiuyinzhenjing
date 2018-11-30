package html
import(
	"net/http"
	"fmt"
)
func Title(w http.ResponseWriter,title string){
	fmt.Fprintf(w,"<title>%s</title>",title)
}