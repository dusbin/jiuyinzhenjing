# 编译
1. make bin 编译主程序
2. make plugin1 编译插件1
3. make plugin2 编译插件2
4. 运行`./bin`
# 插件相关介绍
在go1.8出现以前，一直觉得go语言不能像C/C++一样可以使用动态库的方式动态修改服务。每次升级操作都不得不重新编译整个工程，重新部署服务。这对于很多比较重型的服务来说是一个很致命的弱点。
目前在go1.8只在linux和Darwin系统下支持plugin.从go1.8源码中plugin包中plugin.go文件开头中有对应的说明。在go1.8中plugin包在操作系统的支持并不十分完善，即使在linux系统下也需要特定gcc的编译器及连接器的支持。后续版本应该会有做相应的改进。 
# 参考资料
https://blog.csdn.net/m0_38132420/article/details/68496881