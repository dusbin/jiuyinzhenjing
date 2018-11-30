package container

import (
	log "github.com/Sirupsen/logrus"
	"os"
	"os/exec"
	"syscall"
	"strings"
	"fmt"
)
/*
 *这里是父进程，也就是当前进程执行的内容，
 * 1./proc/self/exe调用中,/proc/self/指的就是当前运行进程自己的环境，exec其实就是自己调用自己，对创建出来的进程进行初始化
 * 2.arg是参数，其中init是传递给本进程的第一个参数，其实就是调用initCommand去初始化进程的一些环境和资源
 * 3.下面的clone参数就是fork出来一个进程，并且使用了namespace隔离新创建的进程和外部环境
 * 4.如果指定了ti参数就需要把当前进程的输入输出导入到标准输入输出上
 */
func NewParentProcess(tty bool,volume string) (*exec.Cmd, *os.File) {
	readPipe, writePipe, err := NewPipe()  //函数返回多个变量
	if err != nil {
		log.Errorf("New pipe error %v", err)
		return nil, nil
	}
	cmd := exec.Command("/proc/self/exe", "init")
	fmt.Printf("NewParentProcess\n")
    cmd.SysProcAttr = &syscall.SysProcAttr{
        Cloneflags: syscall.CLONE_NEWUTS | syscall.CLONE_NEWPID | syscall.CLONE_NEWNS |
		syscall.CLONE_NEWNET | syscall.CLONE_NEWIPC,
    }
	if tty {
		cmd.Stdin = os.Stdin
		cmd.Stdout = os.Stdout
		cmd.Stderr = os.Stderr
	}
	//传入管道文件读取端的句柄
	//主要解决的问题是如何将创建的管道的一端传给子进程，这是一个文件类型，肯定不能通过字符参数的方式进行传递，
	//因此使用了command的cmd.ExtraFiles，这个属性的意思是会外带着这个文件句柄去创建子进程，为什么叫“外带着”
	//呢？因为1个进程默认会有3个文件描述符，分别是标准输入，标准输出，标准错误。这3个是子进程一创建的时候就会
	//默认携带，外带的这个文件描述符理所当然的成为第四个
	cmd.ExtraFiles = []*os.File{readPipe}
	//cmd.Dir = "/root/busybox"   /*应该是这个路径下没有可执行的文件，不能使用，设置为根目录可以正常使用*/
	//cmd.Dir = "/"，
	//mntURL := "/root/mnt/"
	//rootURL := "/root/"
	mntURL := "/root/mnt"
	rootURL := "/root"
	NewWorkSpace(rootURL, mntURL, volume)
	cmd.Dir = mntURL
	return cmd, writePipe
}
//pipe方法生成一个匿名管道，这个函数返回两个变量，一个是读一个是写，其类型都是文件类型
func NewPipe() (*os.File, *os.File, error) {
	read, write, err := os.Pipe()
	if err != nil {
		return nil, nil, err
	}
	return read, write, nil
}

//Create a AUFS filesystem as container root workspace
/*
 * 创建容器文件系统
 * 创建容器文件系统的过程如下。
 *  1.创建只读层。
 *  2.创建容器读写层。
 *  3.创建挂载点井把只读层和读写层挂载到挂载点上。
 *  4.接下来，首先判断volume是否为空，如果是，就表示用户并没有使用挂载标签，结束创建过程。
 *  5.如果不为空，则使用volumeUrlExtract函数解析volume字符串。
 *  6.当volumeUr!Extract函数返回的字符数组长度为2，并且数据元素均不为空的时候，则执行MountVolume函数来挂载数据卷。
 *  7.否则，提示用户创建数据卷输入值不对。
 */
func NewWorkSpace(rootURL string, mntURL string, volume string) {
	CreateReadOnlyLayer(rootURL) //创建只读层，即/root/busybox
	CreateWriteLayer(rootURL) //创建数据层
	CreateMountPoint(rootURL, mntURL) //创建挂载点
	/*
	 * volume为空，没有使用挂载标签 -v
	 * 不为空，使用volumeUrlExtract进行解析 volume为 volumeURLs
	 * 获取正确，使用MountVolume进行挂载数据卷
	 */
	if(volume != ""){
		volumeURLs := volumeUrlExtract(volume)
		length := len(volumeURLs)
		if(length == 2 && volumeURLs[0] != "" && volumeURLs[1] !=""){
			MountVolume(rootURL, mntURL, volumeURLs)
			log.Infof("%q",volumeURLs)
		}else{
			log.Infof("Volume parameter input is not correct.")
		}
	}
}

func CreateReadOnlyLayer(rootURL string) {
	busyboxURL := rootURL + "/busybox"
	busyboxTarURL := rootURL + "/busybox.tar"
	exist, err := PathExists(busyboxURL)
	if err != nil {
		log.Infof("Fail to judge whether dir %s exists. %v", busyboxURL, err)
	}
	if exist == false {
		if err := os.Mkdir(busyboxURL, 0777); err != nil {
			log.Errorf("Mkdir busybox dir %s error. %v", busyboxURL, err)
		}
		if _, err := exec.Command("tar", "-xvf", busyboxTarURL, "-C", busyboxURL).CombinedOutput(); err != nil {
			log.Errorf("Untar dir %s error %v", busyboxURL, err)
		}
	}
}

func CreateWriteLayer(rootURL string) {
	writeURL := rootURL + "/writeLayer"
	if err := os.Mkdir(writeURL, 0777); err != nil {
		log.Infof("Mkdir write layer dir %s error. %v", writeURL, err)
	}
}
/*
 * 挂载数据卷的过程如下。
 * l.首先，读取宿主机文件目录URL，创建宿主机文件目录（／root/${parentUrl｝）。
 * 2.然后，读取容器挂载点URL，在容器文件系统里创建挂载点（/root/rnnt/${containerUrl｝）。
 * 3.最后，把宿主机文件目录挂载到容器挂载点。这样启动容器的过程，对数据卷的处理也就完成了。
 */
func MountVolume(rootURL string, mntURL string, volumeURLs []string)  {
	/*
	 *创建宿主机文件目录
	 */
	parentUrl := volumeURLs[0]
	if err := os.Mkdir(parentUrl, 0777); err != nil {
		log.Infof("Mkdir parent dir %s error. %v", parentUrl, err)
	}
	/*
	 *在容器文件系统里创建挂载点
	 */
	containerUrl := volumeURLs[1]
	containerVolumeURL := mntURL + containerUrl
	if err := os.Mkdir(containerVolumeURL, 0777); err != nil {
		log.Infof("Mkdir container dir %s error. %v", containerVolumeURL, err)
	}
	/*
	 * 把宿主机文件目录挂载到容器挂载点
	 */
	dirs := "dirs=" + parentUrl
	cmd := exec.Command("mount", "-t", "aufs", "-o", dirs, "none", containerVolumeURL)
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	if err := cmd.Run(); err != nil {
		log.Errorf("Mount volume failed. %v", err)
	}

}

func CreateMountPoint(rootURL string, mntURL string) {
	if err := os.Mkdir(mntURL, 0777); err != nil {
		log.Infof("Mkdir mountpoint dir %s error. %v", mntURL, err)
	}
	dirs := "dirs=" + rootURL + "/writeLayer:" + rootURL + "/busybox"
	cmd := exec.Command("mount", "-t", "aufs", "-o", dirs, "none", mntURL)
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	if err := cmd.Run(); err != nil {
		log.Errorf("Mount mountpoint dir failed. %v", err)
	}
}

//Delete the AUFS filesystem while container exit
/*
 * docker 退出时删除挂载点和数据层的数据
 * 删除容器文件系统的过程如下。
 *  1.只有在volume不为空，并且使用volumeUrlExtract函数解析volume字符串返回的字符数组长
 *    度为2，数据元素均不为空的时候，才执行DeleteMountPointWithVolume函数来处理。
 *  2.其余的情况仍然使用前面的DeleteMountPoint函数。
 */
func DeleteWorkSpace(rootURL string, mntURL string, volume string){
	if(volume != ""){
		volumeURLs := volumeUrlExtract(volume)
		length := len(volumeURLs)
		if(length == 2 && volumeURLs[0] != "" && volumeURLs[1] !=""){
			DeleteMountPointWithVolume(rootURL, mntURL, volumeURLs)
		}else{
			DeleteMountPoint(rootURL, mntURL)
		}
	}else{
		DeleteMountPoint(rootURL, mntURL)
	}
	DeleteWriteLayer(rootURL)
	DeleteReadOnlyLayer(rootURL);
}
/*
 * 删除只读root层
 */
func DeleteReadOnlyLayer(rootURL string){
	busyboxURL := rootURL + "busybox/"
	exist, err := PathExists(busyboxURL)
	if err != nil {
		log.Infof("Fail to judge whether dir %s exists. %v", busyboxURL, err)
	}
	if exist != false{
		if err := os.RemoveAll(busyboxURL); err != nil {
			log.Errorf("Remove dir %s error %v", busyboxURL, err)
		}
	}
}

func DeleteMountPoint(rootURL string, mntURL string){
	cmd := exec.Command("umount", mntURL)
	cmd.Stdout=os.Stdout
	cmd.Stderr=os.Stderr
	if err := cmd.Run(); err != nil {
		log.Errorf("%v",err)
	}
	if err := os.RemoveAll(mntURL); err != nil {
		log.Infof("Remove mountpoint dir %s error %v", mntURL, err)
	}
}
/*
 * 1.首先，卸载volume挂载点的文件系统（/root/mnt/${containerUrl} ），保证整个容器的挂载点没有被使用。
 * 2.然后，再卸载整个容器文件系统的挂载点（/root/mnt）。
 * 3.最后，删除容器文件系统挂载点。整个容器退出过程中的文件系统处理就结束了。
 */
func DeleteMountPointWithVolume(rootURL string, mntURL string, volumeURLs []string){
	containerUrl := mntURL + volumeURLs[1]
	cmd := exec.Command("umount", containerUrl)
	cmd.Stdout=os.Stdout
	cmd.Stderr=os.Stderr
	if err := cmd.Run(); err != nil {
		log.Errorf("Umount volume failed. %v",err)
	}

	cmd = exec.Command("umount", mntURL)
	cmd.Stdout=os.Stdout
	cmd.Stderr=os.Stderr
	if err := cmd.Run(); err != nil {
		log.Errorf("Umount mountpoint failed. %v",err)
	}

	if err := os.RemoveAll(mntURL); err != nil {
		log.Infof("Remove mountpoint dir %s error %v", mntURL, err)
	}
}

func DeleteWriteLayer(rootURL string) {
	writeURL := rootURL + "/writeLayer"
	if err := os.RemoveAll(writeURL); err != nil {
		log.Infof("Remove writeLayer dir %s error %v", writeURL, err)
	}
}


func PathExists(path string) (bool, error) {
	_, err := os.Stat(path)
	if err == nil {
		return true, nil
	}
	if os.IsNotExist(err) {
		return false, nil
	}
	return false, err
}
/*
 * 解析volume字符串
 */
func volumeUrlExtract(volume string)([]string){
	var volumeURLs []string
	volumeURLs =  strings.Split(volume, ":")
	return volumeURLs
}
