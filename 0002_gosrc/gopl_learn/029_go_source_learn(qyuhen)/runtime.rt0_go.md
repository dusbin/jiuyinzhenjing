```
    87	TEXT runtime·rt0_go(SB),NOSPLIT,$0
    88		// copy arguments forward on an even stack
    89		MOVQ	DI, AX		// argc
    90		MOVQ	SI, BX		// argv
    91		SUBQ	$(4*8+7), SP		// 2args 2auto
    92		ANDQ	$~15, SP
    93		MOVQ	AX, 16(SP)
    94		MOVQ	BX, 24(SP)
    95		
    96		// create istack out of the given (operating system) stack.
    97		// _cgo_init may update stackguard.
    98		MOVQ	$runtime·g0(SB), DI
    99		LEAQ	(-64*1024+104)(SP), BX
   100		MOVQ	BX, g_stackguard0(DI)
   101		MOVQ	BX, g_stackguard1(DI)
   102		MOVQ	BX, (g_stack+stack_lo)(DI)
   103		MOVQ	SP, (g_stack+stack_hi)(DI)
   104	
   105		// find out information about the processor we're on
   106		MOVL	$0, AX
   107		CPUID
   108		MOVL	AX, SI
   109		CMPL	AX, $0
   110		JE	nocpuinfo
   111	
   112		// Figure out how to serialize RDTSC.
   113		// On Intel processors LFENCE is enough. AMD requires MFENCE.
   114		// Don't know about the rest, so let's do MFENCE.
   115		CMPL	BX, $0x756E6547  // "Genu"
   116		JNE	notintel
   117		CMPL	DX, $0x49656E69  // "ineI"
   118		JNE	notintel
   119		CMPL	CX, $0x6C65746E  // "ntel"
   120		JNE	notintel
   121		MOVB	$1, runtime·isIntel(SB)
   122		MOVB	$1, runtime·lfenceBeforeRdtsc(SB)
   123	notintel:
   124	
   125		// Load EAX=1 cpuid flags
   126		MOVL	$1, AX
   127		CPUID
   128		MOVL	AX, runtime·processorVersionInfo(SB)
   129	
   130		TESTL	$(1<<26), DX // SSE2
   131		SETNE	runtime·support_sse2(SB)
   132	
   133		TESTL	$(1<<9), CX // SSSE3
   134		SETNE	runtime·support_ssse3(SB)
   135	
   136		TESTL	$(1<<19), CX // SSE4.1
   137		SETNE	runtime·support_sse41(SB)
   138	
   139		TESTL	$(1<<20), CX // SSE4.2
   140		SETNE	runtime·support_sse42(SB)
   141	
   142		TESTL	$(1<<23), CX // POPCNT
   143		SETNE	runtime·support_popcnt(SB)
   144	
   145		TESTL	$(1<<25), CX // AES
   146		SETNE	runtime·support_aes(SB)
   147	
   148		TESTL	$(1<<27), CX // OSXSAVE
   149		SETNE	runtime·support_osxsave(SB)
   150	
   151		// If OS support for XMM and YMM is not present
   152		// support_avx will be set back to false later.
   153		TESTL	$(1<<28), CX // AVX
   154		SETNE	runtime·support_avx(SB)
   155	
   156	eax7:
   157		// Load EAX=7/ECX=0 cpuid flags
   158		CMPL	SI, $7
   159		JLT	osavx
   160		MOVL	$7, AX
   161		MOVL	$0, CX
   162		CPUID
   163	
   164		TESTL	$(1<<3), BX // BMI1
   165		SETNE	runtime·support_bmi1(SB)
   166	
   167		// If OS support for XMM and YMM is not present
   168		// support_avx2 will be set back to false later.
   169		TESTL	$(1<<5), BX
   170		SETNE	runtime·support_avx2(SB)
   171	
   172		TESTL	$(1<<8), BX // BMI2
   173		SETNE	runtime·support_bmi2(SB)
   174	
   175		TESTL	$(1<<9), BX // ERMS
   176		SETNE	runtime·support_erms(SB)
   177	
   178	osavx:
   179		CMPB	runtime·support_osxsave(SB), $1
   180		JNE	noavx
   181		MOVL	$0, CX
   182		// For XGETBV, OSXSAVE bit is required and sufficient
   183		XGETBV
   184		ANDL	$6, AX
   185		CMPL	AX, $6 // Check for OS support of XMM and YMM registers.
   186		JE nocpuinfo
   187	noavx:
   188		MOVB $0, runtime·support_avx(SB)
   189		MOVB $0, runtime·support_avx2(SB)
   190	
   191	nocpuinfo:
   192		// if there is an _cgo_init, call it.
   193		MOVQ	_cgo_init(SB), AX
   194		TESTQ	AX, AX
   195		JZ	needtls
   196		// g0 already in DI
   197		MOVQ	DI, CX	// Win64 uses CX for first parameter
   198		MOVQ	$setg_gcc<>(SB), SI
   199		CALL	AX
   200	
   201		// update stackguard after _cgo_init
   202		MOVQ	$runtime·g0(SB), CX
   203		MOVQ	(g_stack+stack_lo)(CX), AX
   204		ADDQ	$const__StackGuard, AX
   205		MOVQ	AX, g_stackguard0(CX)
   206		MOVQ	AX, g_stackguard1(CX)
   207	
   208	#ifndef GOOS_windows
   209		JMP ok
   210	#endif
   211	needtls:
   212	#ifdef GOOS_plan9
   213		// skip TLS setup on Plan 9
   214		JMP ok
   215	#endif
   216	#ifdef GOOS_solaris
   217		// skip TLS setup on Solaris
   218		JMP ok
   219	#endif
   220	
   221		LEAQ	runtime·m0+m_tls(SB), DI
   222		CALL	runtime·settls(SB)
   223	
   224		// store through it, to make sure it works
   225		get_tls(BX)
   226		MOVQ	$0x123, g(BX)
   227		MOVQ	runtime·m0+m_tls(SB), AX
   228		CMPQ	AX, $0x123
   229		JEQ 2(PC)
   230		MOVL	AX, 0	// abort
   231	ok:
   232		// set the per-goroutine and per-mach "registers"
   233		get_tls(BX)
   234		LEAQ	runtime·g0(SB), CX
   235		MOVQ	CX, g(BX)
   236		LEAQ	runtime·m0(SB), AX
   237	
   238		// save m->g0 = g0
   239		MOVQ	CX, m_g0(AX)
   240		// save m0 to g0->m
   241		MOVQ	AX, g_m(CX)
   242	
   243		CLD				// convention is D is always left cleared
   244		CALL	runtime·check(SB)
   245	
   246		MOVL	16(SP), AX		// copy argc
   247		MOVL	AX, 0(SP)
   248		MOVQ	24(SP), AX		// copy argv
   249		MOVQ	AX, 8(SP)
   //调用参数初始化和os初始化schedinit初始化
   250		CALL	runtime·args(SB)
   251		CALL	runtime·osinit(SB)
   252		CALL	runtime·schedinit(SB)
   253	
   254		// create a new goroutine to start program
   /// 创建main goroutine 用于执行runtime.main
   255		MOVQ	$runtime·mainPC(SB), AX		// entry
   256		PUSHQ	AX
   257		PUSHQ	$0			// arg size
   258		CALL	runtime·newproc(SB)
   259		POPQ	AX
   260		POPQ	AX
   261	
   262		// start this M
   //让当前线程开始执行main goroutine
   263		CALL	runtime·mstart(SB)
   264	
   265		MOVL	$0xf1, 0xf1  // crash
   266		RET
   267	
   268	DATA	runtime·mainPC+0(SB)/8,$runtime·main(SB)
   269	GLOBL	runtime·mainPC(SB),RODATA,$8
```