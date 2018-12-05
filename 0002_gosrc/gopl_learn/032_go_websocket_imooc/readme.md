# go实现千万级websocket消息推送服务
0. 参考资料
    1. https://github.com/owenliang/go-websocket
    2. https://github.com/gorilla/websocket
1. 推送系统
    1. 消息推送给所有用户
    2. 推送系统的技术挑战
        ```
        在线人数100万
        每秒消息1000条
        推送频率 100万*1000条/s =10亿条/秒
        N个直播间：推送频率 N*10亿条/秒
2. 拉模式和推模式
    1. 拉模式（由于有2,3,4的缺点，无法实现千万级的消息推送）
        1. 客户端定时轮询服务端的接口获取最新的数据
        2. 数据更新频率低，则大多数请求是无效的
        3. 在线用户数量多，服务端的查询负载会很高
        4. 定时轮询拉取，无法满足时效性要求
    2. 推模式
        1. 仅在数据更新时才需要推送
        2. 需要维护大量的在线长连接
        3. 数据更新后可以`立即推送`
3. 基于websocket推送
    1. `浏览器支持`的socket编程，轻松维持服务端的长连接
    2. 基于TCP可靠传输之上的协议，`无需开发者关心通讯细节`
    3. 提供了高度抽象的编程接口，`业务开发成本较低`
4. WebSocket协议与交互通信流程
    1. 客户端（浏览器）向服务端（go语言实现的一个websocket）发起http请求
    2. `客户端`发起升级协议为webbsocket的请求
    3. `服务端`向`客户端`发送确认协议转换
    * 底层的tcp/ip协议没有中断 websocket 消息是双向的
    4. `客户端`基于websocket向`服务端`发送一个消息
    5. `服务端`主动给`客户端`基于websocket发送消息
5. 传输原理
    1. 协议升级后，继续复用http的底层socket完成后续通讯
    2. message底层被切分为多个frame帧传输
    3. 编程时只需操作messahe，无需关心frame
    4. 框架底层完成TCP网络IO，websocket协议解析，也不需关心
6. 服务端的技术选型与考虑
    1. NodeJS （websocket是浏览器的模式）
        * 单线程模型，推送性能有限
        * 多进程不适合做推送，需要遍历在线用户集合，多进程连接，被散列在多个进程，不方便做，多线程比较好
    2. C/C++
        * TCP通讯，websocket协议实现成本较高
    3. Go
        1. 多线程，基于协程模型并发，很简单的实现多线程
        2. 编译型语言，运行速度并不慢
        3. 成熟的websocket标准库，无需造轮子
7. 实现HTTP服务端
    1. websocket是http协议升级而来
    2. 使用http标准库快速实现空接口`:/ws`
8. 完成websocket握手
    1. 使用websocket.Upgrader完成协议握手，得到websocket长连接
    2. 操作websocket api，读取客户端消息，然后原样发送回去
9. 封装websocket（缺乏工程化的设计）
    1. 其他代码模块，无法直接操作websocket连接
    2. websocket连接`非线程安全`，并发读/写需要同步手段
    3. 隐藏细节，封装API
        1. 封装Connection结构，隐藏websocket底层连接
        2. 封装Connection的API，提供Send/Read/Close等`线程安全接口`
    4. API原理
        1. SendMessage将消息投递到out channel
        2. ReadMessage从in channel读取消息
        3. out channel的消息谁发送给websocket，in channel消息是哪里来的
    5. 内部原理
        1. 启动读协程，循环读取WebSocket，将消息投递到in channel
        2. 启动写协程，循环读取out channel，将消息写给webSocket
