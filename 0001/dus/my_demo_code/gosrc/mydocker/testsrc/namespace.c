/*
 * 参考资料
 *https://www.linuxprobe.com/docker-linux-namespace-1.html
 *https://www.linuxprobe.com/docker-linux-namespace-2.html
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
//#include <asm/system.h>
#include <unistd.h>
#include <syscall.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sched.h>
#include <sys/utsname.h>
#include <sys/mount.h>
#define STACK_SIZE (1024*1024)
//gcc -o namespace namespace.c -lpthread
static char container_stack[STACK_SIZE];
char* const container_args[] = {
	"/bin/bash",
	//"-1",
	NULL
};
int container_main(void* arg){
	printf("Container - inside the container!\n");
	sethostname("container",10);/*设置hostname*/
	system("mount -t proc proc /proc");/*Mount Namespace 挂载proc*/
	if(mount("proc","/rootfs/proc","proc",0,NULL)!=0){
		perror("proc");
		printf("1\n");
	}
	if(mount("sysfs","/rootfs/sysfs","sysfs",0,NULL)!=0){
		perror("sys");
		printf("2\n");
	}
	if(mount("none","/rootfs/tmp","tmpfs",0,NULL)!=0){
		perror("tmp");
		printf("3\n");
	}
	if(mount("udev","/rootfs/dev","devtmpfs",0,NULL)!=0){
		perror("dev");
		printf("4\n");
	}
	if(mount("devpts","/rootfs/dev/pts","devpts",0,NULL)!=0){
		perror("dev/pts");
		printf("5\n");
	}
	if(mount("shm","/rootfs/dev/shm","tmpfs",0,NULL)!=0){
		perror("dev/shm");
		printf("6\n");
	}
	if(mount("tmpfs","/rootfs/run","tmpfs",0,NULL)!=0){
		perror("run");
		printf("7\n");
	}
	/*
     * 模仿Docker的从外向容器里mount相关的配置文件
     * 你可以查看：/var/lib/docker/containers//目录，
     * 你会看到docker的这些文件的。
     */
    if(mount("conf/hosts", "rootfs/etc/hosts", "none", MS_BIND, NULL)!=0 ||
        mount("conf/hostname", "rootfs/etc/hostname", "none", MS_BIND, NULL)!=0 ||
        mount("conf/resolv.conf", "rootfs/etc/resolv.conf", "none", MS_BIND, NULL)!=0 ) {
        perror("conf");
		printf("8\n");
    }
    /* 模仿docker run命令中的 -v, --volume=[] 参数干的事 */
    if(mount("/tmp/t1", "rootfs/mnt", "none", MS_BIND, NULL)!=0) {
        perror("mnt");
		printf("9\n");
    }
    /* chroot 隔离目录 */
    if( chdir("./rootfs") != 0 || chroot("./") != 0 ){
        perror("chdir/chroot");
		system("chroot /rootfs/ /bin/sh");
		printf("10\n");
    }
	execv(container_args[0],container_args);
	printf("Something's wrong!\n");
	return 1;
}
int main(){
	printf("Parent - start a container!\n");
	int container_pid = clone(container_main,container_stack+STACK_SIZE,CLONE_NEWUTS|CLONE_NEWIPC|CLONE_NEWPID|CLONE_NEWNS|SIGCHLD,NULL);
	waitpid(container_pid,NULL,0);
	printf("Parent - container stopped!\n");
	return 0;
}

