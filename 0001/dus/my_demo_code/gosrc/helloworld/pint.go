package main
// http://ip:82
import (
    //"fmt"
    "image"
	"image/color"
	"image/png"
	"net/http"
    //"log"
	//"os"
	"time"
)
func pic(w http.ResponseWriter, req *http.Request) {
	const (
		dx = 300
		dy = 500
	)
	// 新建一个 指定大小的 RGBA位图
	img := image.NewNRGBA(image.Rect(0, 0, dx, dy))
	for y := 0; y < dy; y++ {
		for x := 0; x < dx; x++ {
	    	// 设置某个点的颜色，依次是 RGBA
			img.Set(x, y, color.RGBA{uint8(x % 256), uint8(y % 256), 0, 255})
		}
	}
	// 图片流方式输出
	w.Header().Set("Content-Type", "image/png")
    png.Encode(w, img)
}
func main() {
	http.HandleFunc("/", pic)
	s := &http.Server{
	Addr:           ":82",
	ReadTimeout:    30 * time.Second,
	WriteTimeout:   30 * time.Second,
	MaxHeaderBytes: 1 << 20}
	s.ListenAndServe()
}