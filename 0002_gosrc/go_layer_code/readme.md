# 编译
make            编译

make clean      清除生成的文件
# 安装
make inst       安装到/bin目录

make uninst     卸载
# 快速编译安装卸载
make install    编译和安装

make uninstall  清除和卸载
# 使用
myserver        启动服务端

myclient        客户端访问
# 测试
make test       对所有的test用例进行测试

用例的书写

    example.go
    在同级目录下书写examp_test.go 
    举例：./myserver/myswitch/myswitch_test.go
    在对应目录下执行 go test . 进行测试
    或者当前目录下执行go test ./myserver/myswitch/
# 目前实现的功能
server端

client端
# 未来要实现的功能
web服务器

具体的业务处理功能

    获取日期模块
    获取进程模块
    获取用户列表模块
