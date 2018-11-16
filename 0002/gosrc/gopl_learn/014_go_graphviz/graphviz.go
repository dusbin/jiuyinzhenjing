/**
 * 需要安装辅助工具 
 * apt-get install graphviz
 * https://www.cnblogs.com/ghj1976/p/4933997.html
 * https://blog.csdn.net/u010598445/article/details/52225663
 * https://github.com/awalterschulze/gographviz
 * https://github.com/awalterschulze/gographviz.git
 */
package main
import(
	"fmt"
	"github.com/awalterschulze/gographviz"
	"bytes"
	"io/ioutil"
	"os/exec"
)
func main(){
	graphAst,_:=gographviz.Parse([]byte(`digraph G{}`))
	graph := gographviz.NewGraph()
	gographviz.Analyse(graphAst,graph)
	graph.AddNode("G","a",nil)//增加节点a
	graph.AddNode("G","b",nil)//增加节点b
	graph.AddEdge("a","b",true,nil)//增加a->b
	//graph.AddEdge("b","a",true,nil)//增加a<-b
	//graph.AddEdge("a","b",false,nil)//增加无向边
	fmt.Println(graph.String())
	ioutil.WriteFile("11.gv",[]byte(graph.String()),0666)
	system("dot 11.gv -T png -o 12.png")
}
func system(s string){
	cmd := exec.Command(`/bin/sh`,`-c`,s)//调用Command函数
	var out bytes.Buffer
	cmd.Stdout = &out//标准输出
	err := cmd.Run()//运行指令，做判断
	if err!=nil{
		fmt.Println(err)
	}
	fmt.Printf("%s",out.String())
}