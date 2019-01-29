# list源码分析
* 实现逻辑，使用双向链表进行实现，在list结构体中包含len元素，直接可以获取元素个数
* 实现的功能
    1. 初始化/新建链表
    2. 当前节点的前/后一个节点
    3. 获取链表长度
    4. 获取链表头/尾元素
    5. lazyinit() 这是一个私有方法，当没有节点时执行初始化
    6. 插入节点/元素，出入元素直接调用插入节点的方法`return l.insert(&Element{Value: v}, at)`
    7. 删除节点/移动节点
    8. 头/尾插元素/节点
    9. 元素移动到头/尾