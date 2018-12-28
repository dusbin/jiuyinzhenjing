# 准备
1. [下载内核源码文件](https://www.kernel.org/)
# .config
1. 可以自己配置.config文件
2. 复制一份当前系统编译时的配置，在`/usr/src/`目录下，其中在`linux-headers-*-generic`目录下存在`.config`文件，复制一份到内核源码目录下
# 编译配置
1. make menuconfig [load]->[ok]->[save]->[ok]->[exit]-[exit]
2. 可能存在某些库未安装，根据提示进行安装
    ```
    apt-get install libncurses5-dev
    apt-get install ncurses-devel
    apt-get install libncurses5-dev
    apt-get install bison
    apt-get install flex
# 编译内核
1. 编译启动映像 `make bzImage -jN` N为多线程编译
2. 编译模块 `make modules -jN`
# 安装内核
1. 安装模块 `make modules_install`
2. 安装内核 `make install`
# 更改启动grub[未操作]
    ```
    mkinitramfs 4.14.14 -o /boot/initrd,img-4.14.14
    update-grub2
# 参考资料
[Linux内核编译](https://blog.csdn.net/u010472607/article/details/79123495)
