package main
//run命令的实现
import (
	"github.com/xianlubird/mydocker/container"
	"github.com/xianlubird/mydocker/cgroups/subsystems"
	"github.com/xianlubird/mydocker/cgroups"
	log "github.com/Sirupsen/logrus"
	"os"
	"strings"
	"fmt"
)


//func Run(tty bool, comArray []string, res *subsystems.ResourceConfig) {
func Run(tty bool, comArray []string, volume string,res *subsystems.ResourceConfig) {
	fmt.Printf("tty %d\n",tty)
	parent,writePipe := container.NewParentProcess(tty, volume)
	if parent == nil {
		log.Errorf("New parent process error")
		return
	}
	/*
	 *start方法是调用前面创建好的command
	 */
	if err := parent.Start(); err != nil {
		log.Error(err)
		//fmt.Printf("err %s\n",err)
	}
	// use mydocker-cgroup as cgroup name
	// 创建cgroup manager，并通过调用set和apply设置资源限制并使限制在容器上生效
	fmt.Println("begin NewCgroupManager")
	cgroupManager := cgroups.NewCgroupManager("mydocker-cgroup")
	fmt.Println("end NewCgroupManager")
	defer cgroupManager.Destroy()
	//设置资源限制
	cgroupManager.Set(res)
	//将容器进程加入到各个subsystem挂载对应的cgroup中
	cgroupManager.Apply(parent.Process.Pid)
	//对容器设置完成限制后初始化容器
	fmt.Println("begin sendInitCommand")
	sendInitCommand(comArray, writePipe)
	fmt.Println("end sendInitCommand")
	if tty {
		parent.Wait()
	}
	//mntURL := "/root/mnt/"
	//rootURL := "/root/"
	mntURL := "/root/mnt"
	rootURL := "/root"
	container.DeleteWorkSpace(rootURL, mntURL, volume)
	os.Exit(0)
}

func sendInitCommand(comArray []string, writePipe *os.File) {
	command := strings.Join(comArray, " ")
	log.Infof("command all is %s", command)
	writePipe.WriteString(command)
	writePipe.Close()
}
