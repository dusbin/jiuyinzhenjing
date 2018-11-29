# 使用方法
## 编译程序
* 编译前请解决[notes.md](./notes.md)问题
* `make help` 查看需要编译的模块
* `make bin` 编译主模块
* `make run` 运行主模块（生成二进制程序到临时目录）
## 启动服务
* `make run` 或者`./main`
## 访问网址
* http://127.0.0.1:80/
* https://127.0.0.1:8080/
* 访问ip为ifconfig列出的所有ip
## 主模块功能
* index 主目录 
* upload 上传文件到main程序所在目录
* uploadfile 上传文件的结果
## 插件化模块列表
* osinfo 系统信息
* userinfo 当前登录用户
* langinfo 当前系统语言
* svgdemo sin(x)/x的3D图像