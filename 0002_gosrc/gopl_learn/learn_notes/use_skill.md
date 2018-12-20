# 性能测试
1. `go test -bench=.` #基础的
2. `go test -bench=. -cpuprofile cpu.prof` #输出CPU使用信息
    
    `go tool pprof -svg cpu.prof > cpu.svg` #生成CPU使用图svg图片
3. `go test -bench=. -trace trace.out`

    `go tool trace trace.out`
    
    在网页上查看
# 参考资料
[Go 代码重构：23 倍性能提升！](https://studygolang.com/articles/13553?fr=sidebar)
