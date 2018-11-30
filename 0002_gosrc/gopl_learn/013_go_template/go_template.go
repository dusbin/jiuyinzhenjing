package main
import(
	"bytes"
	"fmt"
	"text/template"
	"time"
)
func FormatNow(format string) string{
	return time.Now().Format(format)
}
func main(){
	tmpl:=template.New("t1") //模板名称
	tmpl = tmpl.Funcs(template.FuncMap{"formatNow":FormatNow}) //模板使用的函数 line8
	tmpl,err := tmpl.Parse("hello,{{.}}{{formatNow \"2006-01-02\"}}")//模板
	if err != nil{
		panic(err)
	}
	var doc bytes.Buffer
	name:= "duzhengbin"
	err = tmpl.Execute(&doc,name)
	if err != nil{
		panic(err)
	}
	fmt.Println(doc.String())
	fmt.Println(FormatNow("2006-01-02"))
}