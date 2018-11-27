# 背景
在golang中 需要同时监听多个端口 对http请求进行相同的处理(handler)

# 实现

## 代码如下 很简单
```
func handler1(w http.ResponseWriter, r *http.Request) {
    fmt.Fprintf(w, "Hello!")
}
func main() {
    ports := []string{":25000", ":25001"}
    for _, v := range ports {
        go func(port string) { //每个端口都扔进一个goroutine中去监听
            mux := http.NewServeMux()
            mux.HandleFunc("/", handler1)
            http.ListenAndServe(port, mux)
        }(v)
    }
    select {}
}
```
## 注意点

 *  如果直接在goroutine中使用range出来的value 会提示range variable v captured by func literal
```
go func() { 
    mux := http.NewServeMux()
    mux.HandleFunc("/", handler1)
    http.ListenAndServe(v, mux)
}()
```

* 当goroutine中使用到v时，此时v的只都会变成list中最后一个的数值
详情可见：https://segmentfault.com/a/1190000010884717

* 不使用select{}
for循环后 生成goroutine 然后就直接退出了 0.0 这当然不是我们想要的
所以加一个可以让代码停下来的

* 当然 如果想对不同端口接收到的信息进行不同的处理 就需要自行再加上一些判断啦
--------------------- 
作者：alphaTao 
来源：CSDN 
原文：https://blog.csdn.net/zkt286468541/article/details/81203046 
版权声明：本文为博主原创文章，转载请附上博文链接！