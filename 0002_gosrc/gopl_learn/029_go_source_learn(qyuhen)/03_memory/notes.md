# 内存分配
```
内置运行时的编程语言通常会抛弃传统的内存分配方式，改由自主管理，这样可以完成类似预分配，内存池等操作，以避开系统调用带来的性能问题。可以更好地配合垃圾回收
```
## 基本策略
1. 每次从操作系统申请一大块内存（比如1MB），以减少系统调用
2. 将申请到的大块内存按照特定大小预先切分成小块，构成链表
3. 为对象分配内存时，只需从大小合适的链表提取一个小块即可
4. 回收对象内存时，将该小块内存重新归还到原链表，以便复用
5. 如闲置内存过多，则尝试归还部分内存给操作系统，以降低整体开销
    ```
    内存分配器只管理内存块，不关心对象状态，不会主动回收内存，由垃圾回收器在完成清理操作后，出发内存分配器回收操作
## 内存块
1. 分配器将其管理的内存块分为两种
    * span 由多个地址炼狱的页(page)组成的大块内存
    * object 将span按特定大小切分成多个小块，每个小块可存储一个对象
    ```
    按照用途，span面向内部管理，object面向对象分配
2. 分配器按页数区分不同大小的span，以页为单位将span存放到管理数组中，需要时就以页数为索引进行查找，当然，span大小并非固定不变。在获取闲置span时，如果没找到大小合适的，那就返回页数更多的，此时会引发裁剪操作，多余部分将构成新的span被放回管理数组，分配器还会尝试将地址相邻的空闲span合并，以构建更大的内存块，减少碎片，提供更灵活的分配策略
    * malloc.go
        ```
        _PageShift = 13
        _pageSize  = 1 << _PageShift //8KB
    * mheap.go
        ```
        type msapn struct{
            next *mspan //双向链表
            prev *mspan 
            start pageID //起始序号 = (address >> _PageShift)
            npages uintptr //页数
            freelist golinkptr // 待分配的object链表
        }
3. 用于存储对象的object，按8字节倍数分为n种，比如：大小为24的object可用来存储范围在17~24字节的对象。这种方式虽然会造成一些内存浪费，但分配器只需面对有限的几种规格(size class)小块内存，优化了分配和复用管理策略
    ```
    分配器会尝试将多个微小对象组合到一个object块内、以节约内存
    * malloc.go
        ```
        _NumSizeClasses = 67
4. 分配器初始化时，会构建对照表存储大小和规格的对应关系，包括用来切分的span页数。
    * msize.go
        ```
        // Size classes. Computed and initialized by InitSizes.
        //
        // SizeToClass(0 <= n <= MaxSmallSize) returns the size class,
        // 1 <= sizeclass < MaxSmallSize, for n
        // Size class 0 is reserved to meam "not small".
        // 
        // class_to_size[i] = largest size in class i
        // class_to_allocnpages[i] = number of pages to allocate when
        // makeing new objects in class i
        var class_to_size [_NumSizeClasses]int32
        var class_to_allocnpages [_NumSizeClasses]int32
        var size_to_class8 [1024/8 + 1]int8
        var size_to_class128[(_MaxSmallSize-1024)/128 + 1]int8
5. 对象大小超出特定阈值限制，会被当做大对象(large object)特别对待
    * malloc.go
        ```
        _MaxSmallSize = 32 << 10 //32KB
## 管理组件
1. 优秀的内存分配器必须要在性能和内存利用率之间做到平衡，go采用tcmalloc架构
    * malloc.go
        ```
        // Memory allocator,based on tcmalloc
        // http://goog-perftools.sourceforge.net/doc/tcmalloc.html
2. 分配器有三种组件组成
    * cache 每个运行期工作现场都会绑定一个cache，用于无锁object分配
    * central 为所有cache提供切分好的后备span资源
    * heap 管理限制span，需要时向操作系统申请新内存
    * mheap.go
        ```
        type mheap struct{
            free [_MaxMHeapList]mspan //页数在127以内的闲置span链表数组
            freelarge mspan //页数大于127(>=1MB)大span链表
            //每个central对应一致sizeclass
            central [_NumSizeClasses] struct{
                mcentral mcentral
            }
        }
    * mcentral.go
        ```
        type mcentral struct{
            sizeclass int32 //规格
            nonempty mspan //链表 尚有空闲object的span
            empty mspan //链表 没有空闲object，或已被cache取走的span
        }
    * mcache.go
        ```
        type mcacke struct {
            alloc [_NumSizeClasses]*mspan //以sizeclass为所有管理多个用于分配的span
        }
3. 分配流程
    1. 计算待分配对象对应规格(size class)
    2. 从cache.alloc数组找到规格相同的span
    3. 从span.freelist链表提取可用object
    4. 如span.freelist为空，从central获取新span
    5. 如central.nonempty为空，从heap.free/freelarge获取，并切分成object链表
    6. 如heap没有大小合适的闲置span，向操作系统申请新内存块
4. 释放流程
    1. 将标记为可回收object交还给所属span.freelist
    2. 该span被放回central，可供任意cache重新获取使用
    3. 如span已收回全部object，则将其交还给heap，以便重新切分复用
    4. 定期扫描heap里长时间闲置的span，释放其占用内存
        ```
        大对象直接从heap分配和回收
5. 作为工作线程私有且不被共享的cache是实现高性能无锁分配的核心，而central的作用是在多个cache间提高object利用率，避免内存浪费
    ```
    加入cache1获取一个span后，仅使用了一部分object，那么剩余空间就可能会被浪费，而回收操作将该span交还给central后，该span完全可以被cache2，cacheN获取使用，此时，cache1已不再持有该span，完全不会造成问题
6. 将span归还给heap，是为了在不同规格object需求间平衡
    ```
    某时段某种规格的object需求量可能激增，那么当需求过后，大量被切分成该规格的span就会被闲置浪费，将归还给heap，就可被其他需求获取，重新切分
## 初始化
1. 因为内存分配器和垃圾回收算法都依赖连续地址，所以在初始化阶段，预先保留了很大的一段虚拟地址空间
    ```
2. 保留地址空间，并不会分配内存
该段空间被划分成三个区域
    ```
    页所属span指针数组  gc标记位置  用户内存分配区域
    +----------------+----------+---------------+
    | span 512MB     |bitmap32GB| arena 512GB   |
    +----------------+----------+---------------+
3. 三个数组组成一个高性能内存管理结构
    1. 使用arena地址指向操作系统申请内存，其大小决定了可分配用户内存上限
    2. 位图bitmap为每个对象提供4bit标记位，用以保存指针，gc标记等信息
    3. 创建span时，按页填充对呀span空间，在回收object时，只需将其地址按页对齐后就可以找到所属span，分配器还用此访问相邻span，做合并操作
        ```
        任何arena区域的地址，只要将其偏移量配以不同步幅和起始位置，就可以快速访问与之对应的span、bitmap数据，最关键的是，这三个数组可以按需同步线性扩张，无需预先分配内存
    4. 这些区域相关属性被保存在heap里，其中包括递进的分配位置mapped/used
    * mheap.go
        ```
        type mheap struct{
            spans **mspan
            spans_mapped uintptr
            bitmap uintptr
            bitmap_mapped uintptr
            arena_start uintptr
            arena_used  uintptr
            arena_end   uintptr
            arena_reserved bool
        }
    5. 初始化过程
        1. 创建对象规格大小对照表
        2. 计算相关区域大小，并尝试从某个指定位置开始保留地址空间
        3. 在heap里保存区域信息，包括起始位置和大小
        4. 初始化heap其他属性
        5. 参考runtime/malloc.go的func mallocinit()函数
    6. 根据例子01_sample_learning学习内存变化
        1. 尽管初始化时预留了544GB的虚拟地址空间，但并没有分配内存
        2. 操作系统大多采用机会主义分配策略，申请内存时，仅承诺但不立即分配物理内存
        3. 物理内存分配发送在写操作导致缺页异常调度时，按页提供
## 分配
1. 为对象分配内存需区分在栈还是堆上完成，通常情况下，编译器有责任尽可能使用寄存器和栈来存储对象，这有助于提升性能，减少垃圾回收器压力
2. new函数刈分配在堆上 参考例子02_new_memory
    1. 关闭内联优化 `go build -gcflags "-l" -o test test.go`
    2. `go tool objdump -s "main\.test" test`
        ```
        test.go:4		0x44c150		64488b0c25f8ffffff	MOVQ FS:0xfffffff8, CX			
        test.go:4		0x44c159		483b6110		CMPQ 0x10(CX), SP			
        test.go:4		0x44c15d		7639			JBE 0x44c198				
        test.go:4		0x44c15f		4883ec18		SUBQ $0x18, SP				
        test.go:4		0x44c163		48896c2410		MOVQ BP, 0x10(SP)			
        test.go:4		0x44c168		488d6c2410		LEAQ 0x10(SP), BP			
        test.go:5		0x44c16d		488d05acac0000		LEAQ 0xacac(IP), AX			
        test.go:5		0x44c174		48890424		MOVQ AX, 0(SP)				
        test.go:5		0x44c178		e8a3effbff		CALL runtime.newobject(SB)	分配在堆上	
        test.go:5		0x44c17d		488b442408		MOVQ 0x8(SP), AX			
        test.go:6		0x44c182		48c700bbaa0000		MOVQ $0xaabb, 0(AX)			
        test.go:7		0x44c189		4889442420		MOVQ AX, 0x20(SP)			
        test.go:7		0x44c18e		488b6c2410		MOVQ 0x10(SP), BP			
        test.go:7		0x44c193		4883c418		ADDQ $0x18, SP				
        test.go:7		0x44c197		c3			RET					
        test.go:4		0x44c198		e8d383ffff		CALL runtime.morestack_noctxt(SB)	
        test.go:4		0x44c19d		ebb1			JMP main.test(SB)	
    3. `go build -o test test.go`
    4. `go tool objdump -s "main\.main" test`
        调用内联优化后的代码没有调用newobject在堆上分配内存
        ```
        test.go:9		0x44c150		64488b0c25f8ffffff	MOVQ FS:0xfffffff8, CX			
        test.go:9		0x44c159		483b6110		CMPQ 0x10(CX), SP			
        test.go:9		0x44c15d		7634			JBE 0x44c193				
        test.go:9		0x44c15f		4883ec10		SUBQ $0x10, SP				
        test.go:9		0x44c163		48896c2408		MOVQ BP, 0x8(SP)			
        test.go:9		0x44c168		488d6c2408		LEAQ 0x8(SP), BP			
        test.go:10		0x44c16d		e88e59fdff		CALL runtime.printlock(SB)		
        test.go:10		0x44c172		48c70424bbaa0000	MOVQ $0xaabb, 0(SP)			
        test.go:10		0x44c17a		e80161fdff		CALL runtime.printint(SB)		
        test.go:10		0x44c17f		e80c5cfdff		CALL runtime.printnl(SB)		
        test.go:10		0x44c184		e8f759fdff		CALL runtime.printunlock(SB)		
        test.go:11		0x44c189		488b6c2408		MOVQ 0x8(SP), BP			
        test.go:11		0x44c18e		4883c410		ADDQ $0x10, SP				
        test.go:11		0x44c192		c3			RET					
        test.go:9		0x44c193		e8d883ffff		CALL runtime.morestack_noctxt(SB)	
        test.go:9		0x44c198		ebb6			JMP main.main(SB)	
    5. 编译器这么做，没有内联时，需要在两个栈帧间传递对象，因此在堆上分配而不是返回一个失效栈帧里的数据，内联后实际上成了main栈帧内的局部变量，无需去堆上操作
        ```
        golang编译器支持逃逸分析，它会在编译期通过构建调用图来分析局部变量是否会被外部引用，从而决定是否可直接分配在栈上
        编译参数 -gcflags "-m"可输出编译优化信息，其中包括内联和逃逸分析
        Zero Garbage 就是避免在堆上的分配行为，从而减小垃圾回收压力，提升性能，另外，做性能测试时使用 go test -benchmem参数可以输出堆分配次数统计
    6. newobject如何为对象分配内存的
        * mcache.go 定义cache的结构
            ```
            type mcache struct{
                //Allocator cache for tiny object w/o pointers.
                tiny unsafe.Pointer
                tinyoffset uintptr
                alloc [_NumSizeClasses]*mspan
            }
        * newobject函数代码参见malloc.go
            1. 大对象直接从heap获取span
            2. 小对象从cache.alloc[sizeclass].freelist获取object
            3. 微小对象组合使用cache.tiny object
    7. 对微小对象的处理，
        * 首先，不能是指针,因为多个小对象被组合到一个object里，无法应对垃圾扫描
        * 其次，从span.freelist获取一个16字节的object，然后利用偏移量来记录下次分配的位置
    8. 分配算法
        1. 资源不足时扩张，大对象分配过程没有central环节
        2. 在heap里闲置的span不会被垃圾回收器关注，但central里的span却可能正在被清理，所以当cache从central提取span时，该属性非常重要
        3. 从central里获取span时，优先取用已有资源，哪怕是要执行清理操作，只有当现有资源无法满足，才去heap获取span，并重新切分成object链表
        4. 从heap获取span的算法核心是找到大小最合适的块，首先从页数相同的链表查找，如果没有结果，再从页数更多的链表提取，直至超大块或申请新块
        5. 如返回更大的span，为了避免浪费，会将多余部分切出来重新放回heap链表，同时还尝试合并相邻闲置span空间，减少碎片
    9. freelarge只是一个简单链表，没有页数做索引，也不曾按大小排序，所以只能遍历真个链表，然后选出最小，地址最靠前的块
## 回收
1. 内存回收的源头是垃圾清理操作
2. 内存分配器的核心是内存复用，不再使用的内存会被放回合适位置，等下次分配时再次使用，只有当空闲内存资源过多时，才会考虑释放
3. 基于效率考虑，回收操作不会直接盯着单个对象，而是以span为基本单位，通过比对bitmap里的扫描标记，逐步将object收归原span，最终上交central或heap复用
4. 清理函数sweepone(文件mgcsweep.go)调用mSpan_Sweep来引发内存分配器回收流程
5. 遍历span，将收集到的不可达object合并到freelist链表，如果该span已收回全部object，那么就将这块完全自由的内存还给heap，以便后续复用。
6. 无论是向操作系统申请内存，还是清理回收内存，只要往heap里放span，都会尝试合并左右相邻的闲置span，构成更大的自由块
7. 回收结束，这些被收回的span并不会被释放，而是等待复用
## 释放
1. 在运行时入口函数main.main里会专门启动一个监控任务sysmon，它每隔一段时间就会检查heap里的闲置内存块
2. 遍历free、freelarge里所有span，如果闲置时间超过阈值，释放其关联的物理内存
3. 所谓物理内存释放，另有玄虚
    ```
    系统调用madvise告知操作系统某段内存暂不使用，建议内核回收对呀物理内存，当然这只是个建议，释放回收由内核决定，如果物理内存资源充足，该建议可能会被忽略，以避免无谓的损耗，而当再次使用该内存块时，会发生缺页异常，内核会自动重新关联物理内存页
    分配器面对的是虚拟内存，所以在地址空间充足的情况下，根本无需放弃这段虚拟内存，无需回收mspan等管理对象，这也是arena能线性扩张的根本原因。windows并不支持madvise机制，须在获取span时主动不上被VirtualFree掉的内存
    