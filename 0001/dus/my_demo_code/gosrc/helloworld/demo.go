package main
import (
	"fmt" /*使用格式化的IO*/
	//"unicode/utf8"
	log "./logrus" //当前路径下的logrus目录
	cli "./cli"
	"os"
	)
func main(){
	fmt.Printf("begin...\n");
	log.Info("log---Info")
	app := cli.NewApp()
	app.Flags = []cli.Flag{
		cli.StringFlag{
			Name:	"lang",
			Value:	"english",
			Usage:	"language for the greeting",
		},
	}
	app.Action = func(c *cli.Context)error{
		name:=	"Nefertiti"
		if c.NArg() > 0 {
			name = c.Args().Get(0)
		}
		if c.String("lang") == "spanish" {
			fmt.Println("Hola,",name)
		}else if c.String("lang") == "chinese"{
			fmt.Println("你好，",name)
		}else{
			fmt.Println("Hello,",name)
		}
		return nil
	}
	app.Run(os.Args)
	log.Debug("log---Debug")
	log.Error("log---Error")
	fmt.Printf("end...\n");
}

/*
 * ./demo dus
 * ./demo -lang spanish dus
 * ./demo -lang chinese dus
 */
