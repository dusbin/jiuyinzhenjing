# 介绍
profile就是定时采样，收集cpu，内存等信息，进而给出性能优化指导，golang官方提供了golang自己的性能分析工具的用法，也给出了指导
# 环境需求
golang环境、graphviz
# 使用
1. net/http/pprof 方法 可以在网络上查看，参考`profile.go:test_01()`
    http://127.0.0.1:8080/debug/pprof 查看整体信息
    http://127.0.0.1:8080/debug/pprof/profile 可以将cpu profile下载下来观察分析
    从terminal进入profile，进行细致分析
    go tool pprof http://localhost:6060/debug/pprof/profile
    go tool pprof http://localhost:6060/debug/pprof/heap
    go tool pprof http://localhost:6060/debug/pprof/block
2. 写代码的方法
    会生成两个profile，一个是cpu的，一个是内存的。进入proflie 方法
    go tool pprof main profile
    main 代表的是二进制文件，也就是编译出来的可执行文件
    profile 就是上文中生成的profile，可以是cpu_profile, 也可以是mem_profile

    对于cpu_profile 来说，代码开始的时候就可以开始统计了
    mem_profile 部分代码如果写在代码开始的位置是统计不出来的，需要找到一个比较好的位置
# 参考资料
[golang profile用法](https://www.jianshu.com/p/79807449da2d)
