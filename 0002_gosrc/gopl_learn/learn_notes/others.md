## HandleFunc(pattern，handler)
* pattern 路由接口，handler 回调函数
## rpc介绍
* [demo参见../026_go_rpc](../026_go_rpc/)
* rpc 是客户端调用服务端的函数来完成功能。
* go 语言的rpc符合以下特点才会被远程访问，否则会被忽略掉
```
函数必须是导出的（首字母大写）
必须有两个导出类型的参数
第一个参数时接收的参数
第二个参数时返回给客户端的参数，第二个参数必须是指针类型的
函数还需要有一个返回值 error
举例如下
func (t *T)MethodName(argType T1,replyType *T2) error
T T1 T2类型必须能被encoding/gob包编解码
```
### 参考资料
* https://www.cnblogs.com/andyidea/p/6525714.html
## init函数
* 在程序运行前加载，根据包的加载顺序进行执行。包的加载过程遵循于 递归加载