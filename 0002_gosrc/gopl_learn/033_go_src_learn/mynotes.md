# 测试代码
1. 写测试代码 命名测试函数应该具有标识性的名称，如果是修改bug的测试可参考func TestIssue4103的格式命名，注释可以写为bug的链接和bug的简单描述
# 生成汇编代码的三种方法
1. go tool compile -N -l -S test.go
2. go tool compile -N -l test.go

    go tool objdump test.go
3. go build -gcflags -S test.go

    go tool compile和go build -gcflags -S生成的是过程中的汇编，和最终的机器码的汇编可以通过go tool objdump生成
4. 参考资料

    [Golang汇编命令解读](https://www.cnblogs.com/yjf512/p/6132868.html)

# Go编译器简介 [原文](https://github.com/golang/go/blob/master/src/cmd/compile/README.md)
1. 构成Go编译器的关键package包含在cmd/compole目录 gc代表Go compiler 和垃圾回收的GC没有关系
2. 前端两部分
    * 词法分析和语法分析
        ```
        cmd/compile/internal/syntax目录包含了词法分析、语法分析和语法树构造部分，编译器第一阶段工作中，对每个源文件进行词法、语法分析，并生产AST(抽象语法树)
        每个语法树都精确表达了相应的源代码文件，树的节点对应着各个源文件的元素，例如表达式、声明语句，语法树还包含了代码位置信息，为错误报告和调试信息提供支持
    * 类型检查和AST转换
        ```
        cmd/compile/internal/gc 目录包含了编译器AST创建、类型检查、AST转换部分，这个package中包含了一个从C继承的AST定义。这个package的第一要事就是把syntax包的语法树转换成编译器支持的AST来表示，这个步骤看起来略显多余，在将来的版本中可能会被重构
        接下来要做的是类型检查，第一步做名字解析和类型推断，确定对象属于哪个标识符以及表达式的类型，类型检查还包括一些额外操作，例如判断"声明但未使用"的变量，确定函数是否会终止
        某些节点类型的细化会在这部分完成，例如从算术节点中查分出字符串加、无用代码消除、函数调用内联化和逃逸分析
3. 中端 通常包含在第二个阶段，在前端和后端之间引入一个代码优化阶段
4. 后端两部分
    * 通用SSA [官方介绍](https://github.com/golang/go/tree/master/src/cmd/compile/internal/ssa)
        ```
        cmd/compile/internal/gc(SSA转换)
        cmd/compile/internal/ssa(SSA pass和规则)
        这个阶段，AST将转换为静态单赋值(SSA)形式，这是一种具有特定属性的低级中间表示法，更容易优化和生成机器码
        转换期间还要处理内置函数。现代编译器经过多代进化已经很智能，会用大量优化代码替代内置函数，获得更高性能。
        在AST到SSA转换期间，为了方便复用，一些节点也降级为更简单的组件，例如：内置拷贝替换内存移动，并将range重写为for。由于历史原因，其中一些在转换为SSA之前完成，未来计划全部移至这里。
        然后，一系列与机器无关的pass和规则被应用，这些不涉及任何单一计算机体系结构。
        这些环节包括消除无用代码，删除非必须的空值检查以及删除未使用的分支。通用重写规则主要涉及表达式，例如用常量替换、乘法和浮点运算的优化
    * 生成机器代码
        ```
        cmd/compile/internal/ssa(SSA低级化和特定架构处理)
        cmd/internal/obj(机器码生成)
        机器依赖相关的阶段以比较“较低”的操作开始，这些操作将通用变量重写成其特定于机器的变体，例如amd64架构中，可以合并许多加载存储操作
        要注意“低级”的操作运行所有机器特定的重写规则，它也应用了大量优化
        一旦SSA被“低级”处理并且更具体的针对目标体系架构，就要运行最终代码优化的步骤处理了，其中包含了另一个无用代码消除的步骤。
        该步骤会将变量移动到更靠近他们被使用的位置、删除从未被读取的局部变量以及寄存器分配
        该步骤完成的其他重要工作包括堆栈布局和指针分析，堆栈布局将堆栈偏移分配给局部变量，指针分析计算每个GC安全点上的堆栈指针是否仍然是活动的
        在SSA生成阶段结束时，Go函数已转换为sequence，这些sequence最终被转换成机器码
2. 参考资料

    [Go编译器简介](https://studygolang.com/articles/17466)

    [现代编译器架构](https://www.kancloud.cn/digest/xf-llvm/162258)