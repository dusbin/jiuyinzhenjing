package main

import (
	"fmt"
	log "github.com/Sirupsen/logrus"
	"github.com/urfave/cli"
	"github.com/xianlubird/mydocker/container"
	"github.com/xianlubird/mydocker/cgroups/subsystems"
)

var runCommand = cli.Command{//使用cli定义命令相关信息，Name，Usage，Flags，Action
	Name: "run",
	Usage: `Create a container with namespace and cgroups limit
			mydocker run -ti [command]`,
	Flags: []cli.Flag{
		cli.BoolFlag{
			Name:  "ti",
			Usage: "enable tty",
		},
		cli.BoolFlag{
			Name: "d",
			Usage: "detach container",
		},
		//添加-v参数
		cli.StringFlag{
			Name:  "v",
			Usage: "volume",
		},
		cli.StringFlag{
			Name: "m",
			Usage: "memory limit",
		},
		cli.StringFlag{
			Name: "cpushare",
			Usage: "cpushare limit",
		},
		cli.StringFlag{
			Name: "cpuset",
			Usage: "cpuset limit",
		},
	},
	Action: func(context *cli.Context) error {
		if len(context.Args()) < 1 {
			return fmt.Errorf("Missing container command")
		}
		var cmdArray []string
		for _, arg := range context.Args() {
			cmdArray = append(cmdArray, arg)
		}
		tty := context.Bool("ti")
		volume := context.String("v")
		detach := context.Bool("d")
		if tty && detach {
			return fmt.Errorf("ti and d paramter can not both provided")
		}
		resConf := &subsystems.ResourceConfig{
			MemoryLimit: context.String("m"),
			CpuSet: context.String("cpuset"),
			CpuShare:context.String("cpushare"),
		}
		fmt.Printf("before Run function\n")
		//Run(tty, cmdArray, resConf) //使用run.go中的Run函数
		//将volume参数传给run
		Run(tty, cmdArray,volume,resConf)
		fmt.Printf("end Run function\n")
		return nil
	},
}
//初始化命令定义，定义了init的具体操作，此操作为内部方法，禁止外部调用
var initCommand = cli.Command{
	Name:  "init",
	Usage: "Init container process run user's process in container. Do not call it outside",
	Action: func(context *cli.Context) error {
		log.Infof("init come on")
		err := container.RunContainerInitProcess()
		return err
	},
}
var dus = cli.Command{
	Name: 	"dus",
	Usage: 	"my own command",
	Action: func(context *cli.Context) error{
		fmt.Printf("error my own command\n")
		return nil
	},
}
/*
 * 实现commit命令，从命令行获取镜像名称
 */
var commitCommand = cli.Command{
	Name:  "commit",
	Usage: "commit a container into image",
	Action: func(context *cli.Context) error {
		if len(context.Args()) < 1 {
			return fmt.Errorf("Missing container name")
		}
		imageName := context.Args().Get(0)
		//commitContainer(containerName)
		commitContainer(imageName)//实现打包容器文件函数
		return nil
	},
}
