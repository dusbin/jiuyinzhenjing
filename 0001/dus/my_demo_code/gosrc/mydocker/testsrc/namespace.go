package main
import (
	"os/exec"
	"syscall"
	"os"
	"log"
	"path"
	"fmt"
	"io/ioutil"
	"strconv"
)
//挂载memory subsystem的hierarchy的根目录位置
const cgroupMemoryHierarchyMount = "/sys/fs/cgroup/memory"
func main(){
	if os.Args[0] == "/proc/self/exe"{
		//容器进程
		fmt.Printf("current pid %d",syscall.Getpid())
		fmt.Println();
		//须安装stress (apt-get install stress)
		cmd := exec.Command("sh","-c",`stress --vm-bytes 200m --vm-keep -m 1`)
		cmd.SysProcAttr = &syscall.SysProcAttr{
		//		Cloneflags:
		//		syscall.CLONE_NEWUTS|syscall.CLONE_NEWIPC|
		//		syscall.CLONE_NEWPID|syscall.CLONE_NEWNS|
		//		syscall.CLONE_NEWUSER|syscall.CLONE_NEWNET,
		//		//begin for kernel 4.4
		//	UidMappings: []syscall.SysProcIDMap{
		//		{
		//			ContainerID: 	0,
		//			HostID:			syscall.Getuid(),
		//			Size:			1,
		//		},
		//	},
		//	GidMappings: []syscall.SysProcIDMap{
		//		{
		//			ContainerID: 	0,
		//			HostID:			syscall.Getgid(),
		//			Size:			1,				
		//		},
		//	},
		//	//end
		}
		cmd.Stdin = os.Stdin
		cmd.Stdout = os.Stdout
		cmd.Stderr = os.Stderr
		if err := cmd.Run();err != nil{
			log.Fatal(err)
			//fmt.Printf(err)
			os.Exit(1)
		}
	}
	fmt.Printf("dzb\n")	
	//begin from book
	//cmd.SysProcAttr.Credential = &syscall.Credential{Uid: uint32(1000), Gid: uint32(1000)}/*4.4内核上不适用*/
	//end
	cmd := exec.Command("/proc/self/exe")	
	cmd.SysProcAttr = &syscall.SysProcAttr{
			Cloneflags:
			syscall.CLONE_NEWUTS|syscall.CLONE_NEWPID|syscall.CLONE_NEWNS,
	//		syscall.CLONE_NEWUTS|syscall.CLONE_NEWIPC|
	//		syscall.CLONE_NEWPID|syscall.CLONE_NEWNS|
	//		syscall.CLONE_NEWUSER|syscall.CLONE_NEWNET,
	//		//begin for kernel 4.4
	//	UidMappings: []syscall.SysProcIDMap{
	//		{
	//			ContainerID: 	0,
	//			HostID:			syscall.Getuid(),
	//			Size:			1,
	//		},
	//	},
	//	GidMappings: []syscall.SysProcIDMap{
	//		{
	//			ContainerID: 	0,
	//			HostID:			syscall.Getgid(),
	//			Size:			1,				
	//		},
	//	},
	//	//end
	}
	cmd.Stdin = os.Stdin
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	if err := cmd.Start();err != nil{
		fmt.Printf("dzb2\n")	
		log.Fatal(err)
		//fmt.Println("Error",err)
		os.Exit(1)
	}else{
		fmt.Printf("dzb3\n")
		//得到fork出来进程映射在外部命名空间的pid
		fmt.Printf("%v\n",cmd.Process.Pid)
		//在系统默认创建挂载了memory subsystem的Hierarchy上创建cgroup
		os.Mkdir(path.Join(cgroupMemoryHierarchyMount,"testMemorylimit"),0755)
		//将容器进程加入到这个cgroup中
		ioutil.WriteFile(path.Join(cgroupMemoryHierarchyMount,"testMemorylimit","tasks"),[]byte(strconv.Itoa(cmd.Process.Pid)),0644)
		//限制cgroup进程使用
		ioutil.WriteFile(path.Join(cgroupMemoryHierarchyMount,"testMemorylimit","memory.limit_in_bytes"),[]byte("100m"),0644)
	}
	fmt.Printf("dzb\n")	
	cmd.Process.Wait()
	fmt.Printf("dzb\n")	
}
