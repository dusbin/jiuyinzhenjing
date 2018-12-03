```
   477	func schedinit() {
   478		// raceinit must be the first call to race detector.
   479		// In particular, it must be done before mallocinit below calls racemapshadow.
   480		_g_ := getg()
   481		if raceenabled {
   482			_g_.racectx, raceprocctx0 = raceinit()
   483		}
   484	    // 最大系统线程数量限制，参考标准库runtime/debug.SetMaxThreads
   485		sched.maxmcount = 10000
   486	    // 栈、内存分配器、调度器相关初始化
   487		tracebackinit()
   488		moduledataverify()
   489		stackinit()
   490		mallocinit()
   491		mcommoninit(_g_.m)
   492		alginit()       // maps must not be used before this call
   493		modulesinit()   // provides activeModules
   494		typelinksinit() // uses maps, activeModules
   495		itabsinit()     // uses activeModules
   496	
   497		msigsave(_g_.m)
   498		initSigmask = _g_.m.sigmask
   499	    //处理命令行参数和环境变量
   500		goargs()
   501		goenvs()
            // 处理GODEBUG、GOTRACEBACK调试相关的环境变量设置
   502		parsedebugvars()
            // 垃圾回收器初始化
   503		gcinit()
   504	
   505		sched.lastpoll = uint64(nanotime())
            // 通过CPU Core和GOMAXPROCS环境变量确定P的数量
   506		procs := ncpu
   507		if n, ok := atoi32(gogetenv("GOMAXPROCS")); ok && n > 0 {
   508			procs = n
   509		}
            // 调整P数量
   510		if procresize(procs) != nil {
   511			throw("unknown runnable goroutine during bootstrap")
   512		}
   513	
   514		// For cgocheck > 1, we turn on the write barrier at all times
   515		// and check all pointer writes. We can't do this until after
   516		// procresize because the write barrier needs a P.
   517		if debug.cgocheck > 1 {
   518			writeBarrier.cgo = true
   519			writeBarrier.enabled = true
   520			for _, p := range allp {
   521				p.wbBuf.reset()
   522			}
   523		}
   524	
   525		if buildVersion == "" {
   526			// Condition should never trigger. This code just serves
   527			// to ensure runtime·buildVersion is kept in the resulting binary.
   528			buildVersion = "unknown"
   529		}
   530	}

```