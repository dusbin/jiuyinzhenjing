package main


import (
	log "github.com/Sirupsen/logrus"
	"fmt"
	"os/exec"
)
/*
 * 打包函数，入参为镜像名称
 * 实际如命令tar 打包/root/mnt/目录相同，使用命令的形式实现
 */
func commitContainer(imageName string){
	mntURL := "/root/mnt"
	imageTar := "/root/" + imageName + ".tar"
	fmt.Printf("ImageTar name is [%s]\n",imageTar)
	/*调用 tar -czf /root/${imageName}.tar /root/mnt . 进行打包*/
	if _, err := exec.Command("tar", "-czf", imageTar, "-C", mntURL, ".").CombinedOutput(); err != nil {
		log.Errorf("Tar folder %s error %v", mntURL, err)
	}
}