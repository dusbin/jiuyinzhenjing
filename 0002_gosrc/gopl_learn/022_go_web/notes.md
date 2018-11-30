# 编译前需要做的事情
* 解决重复注册HandleFunc问题
```
修改/usr/local/go/src/net/http/server.go中
func (mux *ServeMux) Handle(pattern string, handler Handler) {}函数的
panic("http: multiple registrations for " + pattern)为return,忽略重复注册函数
https://www.cnblogs.com/Bin-DuS/p/10032361.html
```