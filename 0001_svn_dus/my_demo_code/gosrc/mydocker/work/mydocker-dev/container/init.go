package container
//init命令的实现
import (
	"fmt"
	log "github.com/Sirupsen/logrus"
	"io/ioutil"
	"os"
	"os/exec"
	"path/filepath"
	"strings"
	"syscall"
)

func RunContainerInitProcess() error {
	//logrus.Infof("command %s", command)
	fmt.Printf("RunContainerInitProcess\n")
	/*
	 *在本文件系统中不允许允许其他程序 syscall.MS_NOEXEC
	 *在本系统允许程序是，不允许set-user-ID或set-group-ID syscall.MS_NOSUID
	 *这个参数自从linux2.4之后所有mount的系统都会默认设定的参数 syscall.MS_NODEV
	 */
	cmdArray := readUserCommand()
	if cmdArray == nil || len(cmdArray) == 0 {
		return fmt.Errorf("Run container get user command error, cmdArray is nil")
	}

	setUpMount()
	//defaultMountFlags := syscall.MS_NOEXEC | syscall.MS_NOSUID | syscall.MS_NODEV
	//syscall.Mount("proc", "/proc", "proc", uintptr(defaultMountFlags), "")
	path, err := exec.LookPath(cmdArray[0])
	if err != nil {
		log.Errorf("Exec loop path error %v", err)
		return err
	}
	log.Infof("Find path %s", path)
	/*
	 *本函数最后的syscall.Exec，是最为重要的一句黑魔法，正是这个系统调用实现了完成初
	 *始化动作并将用户进程运行起来的操作。你可能会说，这有什么神奇的，不就是运行一下程序
	 *嘛。下面来解释一下这句话的神奇之处。
	 *首先，使用Docker创建起来一个容器之后，会发现容器内的第一个程序，也就是PID为
	 *1的那个进程，是指定的前台进程。那么，根据3.1.1小节所讲的过程发现，容器创建之后，
	 *执行的第一个进程并不是用户的进程，而是init初始化的进程。这时候，如果通过ps命令查
	 *看就会发现，容器内第一个进程变成了自己的init，这和预想的是不一样的。你可能会想，大
	 *不了把第一个进程给kill掉。但这里只有一个令人头疼的问题，PID为1的进程是不能被kill
	 *掉的，如果该进程被kill掉，我们的容器也就退出了，那么有什么办法呢？这里的execve系
	 *统调用就可以大显神威了。
	 *sysca11.Exec这个方法，其实最终调用了Kernel的int execve(const char*filename,char
	 *   *const argv[]、char·const envp[])；这个系统函数。它的作用是执行当前filename对应的程序。它
	 *会覆盖当前进程的镜像、数据和堆栈等信息，包括PID,这些都会被将要运行的进程覆盖掉。
	 *也就是说，调用这个方法，将用户指定的进程运行起来，把最初的init进程给替换掉，这样当
	 *进入到容器内部的时候，就会发现容器内的第一个程序就是我们指定的进程了。这其实也是目
	 *前Docker使用的容器引擎runC的实现方式之一。
	 */
	if err := syscall.Exec(path, cmdArray[0:], os.Environ()); err != nil {
		log.Errorf(err.Error())
		fmt.Printf("Error %s\n",err.Error())
	}
	return nil
}
func readUserCommand() []string {
	//uintptr(3)就是指index为3的文件描述符，也就是传递进来的管道的一端
	pipe := os.NewFile(uintptr(3), "pipe")
	defer pipe.Close()
	msg, err := ioutil.ReadAll(pipe)
	if err != nil {
		log.Errorf("init read pipe error %v", err)
		return nil
	}
	msgStr := string(msg)
	return strings.Split(msgStr, " ")
}

/**
Init 挂载点
*/
func setUpMount() {
	pwd, err := os.Getwd()
	if err != nil {
		log.Errorf("Get current location error %v", err)
		return
	}
	log.Infof("Current location is %s", pwd)
	pivotRoot(pwd)

	//mount proc
	defaultMountFlags := syscall.MS_NOEXEC | syscall.MS_NOSUID | syscall.MS_NODEV
	syscall.Mount("proc", "/proc", "proc", uintptr(defaultMountFlags), "")
	/*
	 * tmpfs是一种基于内存的文件系统，可以使用RAM或swap分区来存储
	 */
	syscall.Mount("tmpfs", "/dev", "tmpfs", syscall.MS_NOSUID|syscall.MS_STRICTATIME, "mode=755")
}

func pivotRoot(root string) error {
	/**
	  为了使当前root的老 root 和新 root 不在同一个文件系统下，我们把root重新mount了一次
	  bind mount是把相同的内容换了一个挂载点的挂载方法
	*/
	if err := syscall.Mount(root, root, "bind", syscall.MS_BIND|syscall.MS_REC, ""); err != nil {
		return fmt.Errorf("Mount rootfs to itself error: %v", err)
	}
	// 创建 rootfs/.pivot_root 存储 old_root
	pivotDir := filepath.Join(root, ".pivot_root")
	if err := os.Mkdir(pivotDir, 0777); err != nil {
		return err
	}
	// pivot_root 到新的rootfs, 现在老的 old_root 是挂载在rootfs/.pivot_root
	// 挂载点现在依然可以在mount命令中看到
	/*
	 *pivotroot是一个系统调用，主要功能是去改变当前的root文件系统。pivotroot可以将当前进程的root文件系统移动到put_old文
	 *件夹中，然后使用new_root成为新的root文件系统，new_root和put_old必须要不能同时存在当前root的同一个文件系统中。pivotroot和
	 *chroot的主要区别是，pivotroot是把整个系统切换到一个新的root目录，而移除对之前root文件系统的依赖，这样就能够umount原来的root文件系统
	 *而chroot是针对某个进程，系统的其他部分依旧运行于老的root目录中。新的文件系统位container_process.go中NewParentProcess函数设置的/root/busybox目录
	 *在容器中touch 一个文件，即在/root/busybox为根目录下进行。使用的是/root/busybox为根目录
	 * /root/busybox为下载好的busybox系统，可以在安装docker的系统中执行docker pull busybox进行镜像下载，使用docker run -d busybox top -b运行容器
	 * 使用docker export -o busybox.tar 容器ID进行导出,
 	 */
	if err := syscall.PivotRoot(root, pivotDir); err != nil {
		return fmt.Errorf("pivot_root %v", err)
	}
	// 修改当前的工作目录到根目录
	if err := syscall.Chdir("/"); err != nil {
		return fmt.Errorf("chdir / %v", err)
	}

	pivotDir = filepath.Join("/", ".pivot_root")
	// umount rootfs/.pivot_root
	if err := syscall.Unmount(pivotDir, syscall.MNT_DETACH); err != nil {
		return fmt.Errorf("unmount pivot_root dir %v", err)
	}
	// 删除临时文件夹
	return os.Remove(pivotDir)
}
