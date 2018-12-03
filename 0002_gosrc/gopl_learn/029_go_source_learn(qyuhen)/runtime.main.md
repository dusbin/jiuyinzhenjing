```
   109	func main() {
   110		g := getg()
   111	
   112		// Racectx of m0->g0 is used only as the parent of the main goroutine.
   113		// It must not be used for anything else.
   114		g.m.g0.racectx = 0
   115	
   116		// Max stack size is 1 GB on 64-bit, 250 MB on 32-bit.
   117		// Using decimal instead of binary GB and MB because
   118		// they look nicer in the stack overflow failure message.
            // 执行栈最大限制：1GB on 64位 250MB on 32位
   119		if sys.PtrSize == 8 {
   120			maxstacksize = 1000000000
   121		} else {
   122			maxstacksize = 250000000
   123		}
   124	
   125		// Allow newproc to start new Ms.
   126		mainStarted = true
   127	    // 启动系统后台监控(定期垃圾回收，以及并发任务调度相关)
   128		systemstack(func() {
   129			newm(sysmon, nil)
   130		})
   131	
   132		// Lock the main goroutine onto this, the main OS thread,
   133		// during initialization. Most programs won't care, but a few
   134		// do require certain calls to be made by the main thread.
   135		// Those can arrange for main.main to run in the main thread
   136		// by calling runtime.LockOSThread during initialization
   137		// to preserve the lock.
   138		lockOSThread()
   139	
   140		if g.m != &m0 {
   141			throw("runtime.main not on m0")
   142		}
   143	    // 执行runtime包内所有初始化函数init
   144		runtime_init() // must be before defer
   145		if nanotime() == 0 {
   146			throw("nanotime returning zero")
   147		}
   148	
   149		// Defer unlock so that runtime.Goexit during init does the unlock too.
   150		needUnlock := true
   151		defer func() {
   152			if needUnlock {
   153				unlockOSThread()
   154			}
   155		}()
   156	
   157		// Record when the world started. Must be after runtime_init
   158		// because nanotime on some platforms depends on startNano.
   159		runtimeInitTime = nanotime()
   160	    // 启动垃圾回收器后台操作
   161		gcenable()
   162	
   163		main_init_done = make(chan bool)
   164		if iscgo {
   165			if _cgo_thread_start == nil {
   166				throw("_cgo_thread_start missing")
   167			}
   168			if GOOS != "windows" {
   169				if _cgo_setenv == nil {
   170					throw("_cgo_setenv missing")
   171				}
   172				if _cgo_unsetenv == nil {
   173					throw("_cgo_unsetenv missing")
   174				}
   175			}
   176			if _cgo_notify_runtime_init_done == nil {
   177				throw("_cgo_notify_runtime_init_done missing")
   178			}
   179			// Start the template thread in case we enter Go from
   180			// a C-created thread and need to create a new thread.
   181			startTemplateThread()
   182			cgocall(_cgo_notify_runtime_init_done, nil)
   183		}
   184	    // 执行所有的用户包(包括标准库)初始化函数init
   185		fn := main_init // make an indirect call, as the linker doesn't know the address of the main package when laying down the runtime
   186		fn()
   187		close(main_init_done)
   188	
   189		needUnlock = false
   190		unlockOSThread()
   191	
   192		if isarchive || islibrary {
   193			// A program compiled with -buildmode=c-archive or c-shared
   194			// has a main, but it is not executed.
   195			return
   196		}
            // 执行用户逻辑入口 main.main函数
   197		fn = main_main // make an indirect call, as the linker doesn't know the address of the main package when laying down the runtime
   198		fn()
   199		if raceenabled {
   200			racefini()
   201		}
   202	
   203		// Make racy client program work: if panicking on
   204		// another goroutine at the same time as main returns,
   205		// let the other goroutine finish printing the panic trace.
   206		// Once it does, it will exit. See issues 3934 and 20018.
   207		if atomic.Load(&runningPanicDefers) != 0 {
   208			// Running deferred functions should not take long.
   209			for c := 0; c < 1000; c++ {
   210				if atomic.Load(&runningPanicDefers) == 0 {
   211					break
   212				}
   213				Gosched()
   214			}
   215		}
   216		if atomic.Load(&panicking) != 0 {
   217			gopark(nil, nil, "panicwait", traceEvGoStop, 1)
   218		}
   219	    // 执行结束，返回退出状态码
   220		exit(0)
   221		for {
   222			var x *int32
   223			*x = 0
   224		}
   225	}
   226	
   227	// os_beforeExit is called from os.Exit(0).
   228	//go:linkname os_beforeExit os.runtime_beforeExit
   229	func os_beforeExit() {
   230		if raceenabled {
   231			racefini()
   232		}
   233	}
```