# 内存空洞 
0. 参考example.go `test_01`
1. 是编译器自动添加的没有被使用的内存空间，用于保证后面每个字段或元素的地址相对
于结构或数组的开始地址能够合理的对齐，内存空洞可能会存在一些随机数据，可能会
对用unsafe包直接操作内存的处理产生影响

如下**为内存空洞
```
0			4			8			
+--+--+--+--+--+--+--+--+
|a |**|b |**************| 8
+-----------------------+
|  c(data)              | 16
+-----------------------+ 
|  c(len)               | 24
+-----------------------+
|  c(cap)               | 32
+-----------------------+
```
		
# type Pointer *ArbitraryType
1. ArbitraryType 不是一个真正的类型，官方导出这个类型出于完善文1档的考虑，在其他
的库和任何项目中都没有使用价值，除非程序员故意使用它
# unsafe.Sizeof unsafe.Alignof unsafe.Offsetof
1. 计算机在加载和保存数据时，如果内存地址合理地对齐的将会更有效率，例如2字节大小的int16类型的变量地址应该是偶数，一个4字节大小的rune类型变量的地址应该是4的倍数，一个8字节大小的float64、uint64或64-bit指针类型变量的地址应该是8字节对齐的，但是对于再大的地址对齐倍数则是不需要的，即使是complex128等较大的数据类型最多也只是8字节对齐
2. 由于地址对齐这个因素，一个聚合类型(结构体或数组)的大小至少是所有字段或元素大小的总和，或者更大因为可能存在内存空洞。内存空洞是编译器自动添加的没有被使用的内存空间，用于保证后面每个字段或元素的地址相对于结构或数组的开始地址能够合理的对齐（内存空洞可能存在一些随机数据，可能会对用unsafe包直接操作内存的处理产生影响）
# unsafe.Pointer介绍
1. 大多数指针类型会写成*T，表示是“一个指向T类型变量的指针”。unsafe.Pointer是特别定义的一种指针类型（类似C语言中的void *类型的指针）它可以包含任意类型变量的地址。当然，不可以直接通过 *p来获取unsafe.Pointer指针指向的真实变量的值，因为我们并不知道变量的具体类型，和普通指针一样，unsafe.Pointer指针也是可以比较的，并且支持和nil常量比较判断是否为空指针
2. 一个普通的*T类型指针可以被转化为unsafe.Pointer类型指针，并且一个unsafe.Pointer类型指针也可以被转回普通的指针，被转回普通的指针类型并不需要和原始的 *T类型相同，通过将 *float64类型指针转化为 *uint64类型指针，我们可以查看一个浮点数变量的位模式
3. 参考example.go `test_02`
4. 通过转为新类型指针，我们可以更新浮点数的位模式，通过位模式操作浮点数是可以的，但是更重要的意义是指针转换语法让我们可以在不破坏类型系统的前提下向内存写入任意的值
5. 一个unsafe.Pointer指针也可以被转化为uintptr类型，然后保存到指针型数值变量中（这只是和当前指针相同的一个数值，并不是一个指针）然后用以做必要的指针数值运算（uintptr是一个无符号的整型数，足以保存一个地址）这种转换虽然也是可逆的，但是将uintptr转为unsafe.Pointer指针可能会破坏类型系统，因为并不是所有的数字都是有效的内存地址
6. 许多将unsafe.Pointer指针转为原生数字，然后再转回为unsafe.Pointer类型指针的操作也是不安全的。
    * 如 `test_03` 写法尽管很繁琐，并不是坏事，因为这些功能应该很谨慎的使用，不要试图引入一个uintptr类型的临时变量，因为它可能会破坏代码的安全性（这是unsafe包不安全的，如`test_04`）
    * test_04 会提示`possible misuse of unsafe.Poniter` 可能存在滥用unsafe.Poniter
    * 产生错误的原因很微妙，有时候垃圾回收器会移动一些变量以降低内存碎片等问题，这类垃圾回收器被移动GC，当一个变量被移动，所有的保存改变量旧地址的指针必须同时被更新为变量移动后的新地址，从垃圾收集器的实际看，一个unsafe.Pointer是一个指向变量的指针，因此当变量被移动是对应的指针也必须被更新，但是uintptr类型的临时变量只是一个普通的数组，所有其值不应该被改变。`test_04`的错误代码因为引入一个非指针的临时变量tmp，导致垃圾收集器无法正确识别这个是一个指向变量x的指针。当第二个语句执行时，变量x可能已经被转移，这时候临时变量tmp也就不再是现在的&x.b地址，第三向之前无效地址空间的赋值语句将彻底毁掉整个程序，存在很大的不确定性
# unsafe.Pointer的使用规则
参考 `test_05`
1. 任何类型的指针都可以被转化为Pointer
2. Pointer可以被转化为任何类型的指针
3. uintptr可以被转化为Pointer
4. Pointer可以被转化为uintptr
# uintptr
1. 底层实现参考runtime/runtime.h
    ```
    #ifdef _64BIT
    typedef uint64          uintptr;
    typedef int64           intptr;
    typedef int64           intgo; // Go's int
    typedef uint64          uintgo; // Go's uint
    #else
    typedef uint32          uintptr;
    typedef int32           intptr;
    typedef int32           intgo; // Go's int
    typedef uint32          uintgo; // Go's uint
    #endif
2. uintptr和intptr是无符号和有符号的指针类型，并且确保在64位平台上是8字节，在32位平台上是4个字节，uinptr主要用于golang中的指针运算
# 合法用例
1. 在[] T和 []MyT之间转换
    * type MyInt int
    * 在golang中，[]int 和 []MyInt是两种不同的类型，因此 []int的值不能转换为 []MyInt,反之一样，但是在unsafe.Pointer的帮助下，转换是可能的 参考 `test_06`
2. 调用sync/atomic包中指针相关的函数
    * sync / atomic包中的以下函数的大多数参数和结果类型都是unsafe.Pointer或*unsafe.Pointer：
        ```
        // CompareAndSwapPointer executes the compare-and-swap operation for a unsafe.Pointer value.
        func CompareAndSwapPointer(addr *unsafe.Pointer, old, new unsafe.Pointer) (swapped bool)

        // LoadPointer atomically loads *addr.
        func LoadPointer(addr *unsafe.Pointer) (val unsafe.Pointer)

        // StorePointer atomically stores val into *addr.
        func StorePointer(addr *unsafe.Pointer, val unsafe.Pointer)

        // SwapPointer atomically stores new into *addr and returns the previous *addr value.
        func SwapPointer(addr *unsafe.Pointer, new unsafe.Pointer) (old unsafe.Pointer)
    * 参考 `test_07`