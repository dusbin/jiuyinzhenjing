package main

import (
	log "github.com/Sirupsen/logrus"
	"github.com/urfave/cli"
	"os"
	"fmt"
)

const usage = `mydocker is a simple container runtime implementation.
			   The purpose of this project is to learn how docker works and how to write a docker by ourselves
			   Enjoy it, just for fun.`

func main() {
	app := cli.NewApp()
	app.Name = "mydocker"
	app.Usage = usage
	app.Version = "ver:1.0.2" //不输入采用cli模块默认值作为版本号
	app.Copyright = "2018-2018"
	app.Author = "duzhengbin"
	app.Email = "847379962@qq.com"
	fmt.Printf("main before []cli.Command\n")
	app.Commands = []cli.Command{//命令列表
		initCommand,
		runCommand,
		commitCommand,//新增加压缩镜像命令
		dus,
	}
	// 删除不影响使用
	fmt.Printf("main before func\n")
	app.Before = func(context *cli.Context) error {//运行命令前初始化logrus日志模块
		// Log as JSON instead of the default ASCII formatter.
		log.SetFormatter(&log.JSONFormatter{}) //已json格式输出
		log.SetOutput(os.Stdout)//输出os.Stdout的信息
		return nil
	}
	//*/
	fmt.Printf("main before Run os.Args = %s\n",os.Args) //os.Args 输入的命令./mydocker run -ti /bin/sh
	if err := app.Run(os.Args); err != nil {//Run执行
		fmt.Printf("Run err\n")
		log.Fatal(err)//Run执行失败会弹出错误信息
	}
	fmt.Printf("main after Run\n")
}
