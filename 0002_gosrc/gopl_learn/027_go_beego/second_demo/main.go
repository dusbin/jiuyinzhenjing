package main

import (
	_ "second_demo/routers"
	"second_demo/controllers"
	"github.com/astaxie/beego"
)

func main() {
	beego.Router("/",&controllers.MainController{})
	beego.Run()
}

