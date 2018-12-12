//+build svgdemo

package main

import (
	"../html"
	"fmt"
	"math"
	"net/http"
)

const (
	width, height = 600, 320            // canvas size in pixels
	cells         = 100                 // number of grid cells
	xyrange       = 30.0                // axis ranges (-xyrange..+xyrange)
	xyscale       = width / 2 / xyrange // pixels per x or y unit
	zscale        = height * 0.4        // pixels per z unit
	angle         = math.Pi / 6         // angle of x, y axes (=30°)
)

var sin30, cos30 = math.Sin(angle), math.Cos(angle) // sin(30°), cos(30°)
/*
	获取插件名称
*/
func Get_plugin_name() (pluginname string) {
	pluginname = "/svgdemo"
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
		1. 生成sin(x)/x的图像
*/
func Func_plugin(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "<html><body>")
	html.Title(w, "svgdemo")
	fmt.Fprintf(w, "<h1>info</h1>")
	fmt.Fprintf(w, "<svg xmlns='http://www.w3.org/2000/svg' style='stroke: red; fill: white; stroke-width: 0.7' width='%d' height='%d'>", width, height)
	for i := 0; i < cells; i++ {
		for j := 0; j < cells; j++ {
			ax, ay := corner(i+1, j)
			bx, by := corner(i, j)
			cx, cy := corner(i, j+1)
			dx, dy := corner(i+1, j+1)
			fmt.Fprintf(w, "<polygon points='%g,%g %g,%g %g,%g %g,%g'/>\n", ax, ay, bx, by, cx, cy, dx, dy)
		}
	}
	fmt.Fprintf(w, "</svg>")
	fmt.Fprintf(w, "</body></html>")
	return
}
func corner(i, j int) (float64, float64) {
	// Find point (x,y) at corner of cell (i,j).
	x := xyrange * (float64(i)/cells - 0.5)
	y := xyrange * (float64(j)/cells - 0.5)
	// Compute surface height z.
	z := f(x, y)
	// Project (x,y,z) isometrically onto 2-D SVG canvas (sx,sy).
	sx := width/2 + (x-y)*cos30*xyscale
	sy := height/2 + (x+y)*sin30*xyscale - z*zscale
	return sx, sy
}
func f(x, y float64) float64 {
	r := math.Hypot(x, y)
	// distance from (0,0)
	return math.Sin(r) / r
}
