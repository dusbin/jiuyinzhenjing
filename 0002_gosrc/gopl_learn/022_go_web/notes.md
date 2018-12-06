# 编译前需要做的事情
* 解决重复注册HandleFunc问题
    ```
    修改go/src/net/http/server.go中
    func (mux *ServeMux) Handle(pattern string, handler Handler) {}函数的
    panic("http: multiple registrations for " + pattern)为return,忽略重复注册函数
    https://www.cnblogs.com/Bin-DuS/p/10032361.html
    ```
* 解决卸载插件依然可以使用插件功能问题
    ```
    go/src/net/http/server.go增加函数
    func CleanServeMuxMap(){
	    for k,_ = range defaultServeMux.m {
		    delete(defaultServeMux.m,k)
	    }
    }
    https://www.cnblogs.com/Bin-DuS/p/10075368.html
    ```