TEXT runtime.init.0(SB) /usr/local/go/src/runtime/cpuflags_amd64.go
  cpuflags_amd64.go:11	0x406480		8b0596083d00		MOVL runtime.processorVersionInfo(SB), AX	
  cpuflags_amd64.go:11	0x406486		25f03fff0f		ANDL $0xfff3ff0, AX				
  cpuflags_amd64.go:13	0x40648b		0fb60d1f083d00		MOVZX runtime.isIntel(SB), CX			
  cpuflags_amd64.go:13	0x406492		84c9			TESTL CL, CL					
  cpuflags_amd64.go:13	0x406494		7425			JE 0x4064bb					
  cpuflags_amd64.go:14	0x406496		3da0060200		CMPL $0x206a0, AX				
  cpuflags_amd64.go:14	0x40649b		751e			JNE 0x4064bb					
  cpuflags_amd64.go:14	0x40649d		b801000000		MOVL $0x1, AX					
  cpuflags_amd64.go:19	0x4064a2		0fb60d0f083d00		MOVZX runtime.support_avx(SB), CX		
  cpuflags_amd64.go:19	0x4064a9		84c9			TESTL CL, CL					
  cpuflags_amd64.go:19	0x4064ab		740a			JE 0x4064b7					
  cpuflags_amd64.go:19	0x4064ad		83f001			XORL $0x1, AX					
  cpuflags_amd64.go:19	0x4064b0		88050d083d00		MOVB AL, runtime.useAVXmemmove(SB)		
  cpuflags_amd64.go:20	0x4064b6		c3			RET						
  cpuflags_amd64.go:20	0x4064b7		31c0			XORL AX, AX					
  cpuflags_amd64.go:19	0x4064b9		ebf5			JMP 0x4064b0					
  cpuflags_amd64.go:15	0x4064bb		3dd0060200		CMPL $0x206d0, AX				
  cpuflags_amd64.go:15	0x4064c0		7507			JNE 0x4064c9					
  cpuflags_amd64.go:15	0x4064c2		b801000000		MOVL $0x1, AX					
  cpuflags_amd64.go:15	0x4064c7		ebd9			JMP 0x4064a2					
  cpuflags_amd64.go:16	0x4064c9		3da0060300		CMPL $0x306a0, AX				
  cpuflags_amd64.go:16	0x4064ce		7507			JNE 0x4064d7					
  cpuflags_amd64.go:16	0x4064d0		b801000000		MOVL $0x1, AX					
  cpuflags_amd64.go:16	0x4064d5		ebcb			JMP 0x4064a2					
  cpuflags_amd64.go:17	0x4064d7		3de0060300		CMPL $0x306e0, AX				
  cpuflags_amd64.go:17	0x4064dc		0f94c0			SETE AL						
  cpuflags_amd64.go:17	0x4064df		ebc1			JMP 0x4064a2					

TEXT runtime.init.1(SB) /usr/local/go/src/runtime/mgcwork.go
  mgcwork.go:25		0x420730		c3			RET			

TEXT runtime.init.2(SB) /usr/local/go/src/runtime/mstats.go
  mstats.go:438		0x425fa0		64488b0c25f8ffffff	MOVQ FS:0xfffffff8, CX			
  mstats.go:438		0x425fa9		488b7110		MOVQ 0x10(CX), SI			
  mstats.go:438		0x425fad		4881fedefaffff		CMPQ $-0x522, SI			
  mstats.go:438		0x425fb4		0f84a9000000		JE 0x426063				
  mstats.go:438		0x425fba		488d842470030000	LEAQ 0x370(SP), AX			
  mstats.go:438		0x425fc2		4829f0			SUBQ SI, AX				
  mstats.go:438		0x425fc5		483d98190000		CMPQ $0x1998, AX			
  mstats.go:438		0x425fcb		0f8692000000		JBE 0x426063				
  mstats.go:438		0x425fd1		4881eca8160000		SUBQ $0x16a8, SP			
  mstats.go:438		0x425fd8		4889ac24a0160000	MOVQ BP, 0x16a0(SP)			
  mstats.go:438		0x425fe0		488dac24a0160000	LEAQ 0x16a0(SP), BP			
  mstats.go:439		0x425fe8		488d7c2410		LEAQ 0x10(SP), DI			
  mstats.go:439		0x425fed		b9d2020000		MOVL $0x2d2, CX				
  mstats.go:439		0x425ff2		31c0			XORL AX, AX				
  mstats.go:439		0x425ff4		f348ab			REP; STOSQ AX, ES:0(DI)			
  mstats.go:440		0x425ff7		488b15cae33500		MOVQ runtime.sizeof_C_MStats(SB), DX	
  mstats.go:440		0x425ffe		4881fa90160000		CMPQ $0x1690, DX			
  mstats.go:440		0x426005		7510			JNE 0x426017				
  mstats.go:440		0x426007		488bac24a0160000	MOVQ 0x16a0(SP), BP			
  mstats.go:440		0x42600f		4881c4a8160000		ADDQ $0x16a8, SP			
  mstats.go:440		0x426016		c3			RET					
  mstats.go:441		0x426017		e8944d0000		CALL runtime.printlock(SB)		
  mstats.go:441		0x42601c		488b05a5e33500		MOVQ runtime.sizeof_C_MStats(SB), AX	
  mstats.go:441		0x426023		48890424		MOVQ AX, 0(SP)				
  mstats.go:441		0x426027		e8f4530000		CALL runtime.printuint(SB)		
  mstats.go:441		0x42602c		e8bf4f0000		CALL runtime.printsp(SB)		
  mstats.go:441		0x426031		48c7042490160000	MOVQ $0x1690, 0(SP)			
  mstats.go:441		0x426039		e8e2530000		CALL runtime.printuint(SB)		
  mstats.go:441		0x42603e		e8fd4f0000		CALL runtime.printnl(SB)		
  mstats.go:441		0x426043		e8e84d0000		CALL runtime.printunlock(SB)		
  mstats.go:442		0x426048		488d0592992400		LEAQ 0x249992(IP), AX			
  mstats.go:442		0x42604f		48890424		MOVQ AX, 0(SP)				
  mstats.go:442		0x426053		48c744240824000000	MOVQ $0x24, 0x8(SP)			
  mstats.go:442		0x42605c		e81f440000		CALL runtime.throw(SB)			
  mstats.go:442		0x426061		0f0b			UD2					
  mstats.go:438		0x426063		e888d00200		CALL runtime.morestack_noctxt(SB)	
  mstats.go:438		0x426068		e933ffffff		JMP runtime.init.2(SB)			

TEXT runtime.init.3(SB) /usr/local/go/src/runtime/panic.go
  panic.go:180		0x429310		64488b0c25f8ffffff	MOVQ FS:0xfffffff8, CX			
  panic.go:180		0x429319		483b6110		CMPQ 0x10(CX), SP			
  panic.go:180		0x42931d		765d			JBE 0x42937c				
  panic.go:180		0x42931f		4883ec18		SUBQ $0x18, SP				
  panic.go:180		0x429323		48896c2410		MOVQ BP, 0x10(SP)			
  panic.go:180		0x429328		488d6c2410		LEAQ 0x10(SP), BP			
  panic.go:181		0x42932d		0f57c0			XORPS X0, X0				
  panic.go:181		0x429330		0f110424		MOVUPS X0, 0(SP)			
  panic.go:182		0x429334		488b0dd5a42600		MOVQ runtime.statictmp_20(SB), CX	
  panic.go:182		0x42933b		488d15dead1d00		LEAQ 0x1dadde(IP), DX			
  panic.go:182		0x429342		48891424		MOVQ DX, 0(SP)				
  panic.go:182		0x429346		48894c2408		MOVQ CX, 0x8(SP)			
  panic.go:183		0x42934b		488b0c24		MOVQ 0(SP), CX				
  panic.go:183		0x42934f		8b154bdc3a00		MOVL runtime.writeBarrier(SB), DX	
  panic.go:183		0x429355		488b4130		MOVQ 0x30(CX), AX			
  panic.go:183		0x429359		85d2			TESTL DX, DX				
  panic.go:183		0x42935b		7511			JNE 0x42936e				
  panic.go:183		0x42935d		488905dcef3800		MOVQ AX, runtime.deferType(SB)		
  panic.go:184		0x429364		488b6c2410		MOVQ 0x10(SP), BP			
  panic.go:184		0x429369		4883c418		ADDQ $0x18, SP				
  panic.go:184		0x42936d		c3			RET					
  panic.go:183		0x42936e		488d3dcbef3800		LEAQ runtime.deferType(SB), DI		
  panic.go:183		0x429375		e876c70200		CALL runtime.gcWriteBarrier(SB)		
  panic.go:183		0x42937a		ebe8			JMP 0x429364				
  panic.go:180		0x42937c		e86f9d0200		CALL runtime.morestack_noctxt(SB)	
  panic.go:180		0x429381		eb8d			JMP runtime.init.3(SB)			

TEXT runtime.init.4(SB) /usr/local/go/src/runtime/proc.go
  proc.go:236		0x42bef0		64488b0c25f8ffffff	MOVQ FS:0xfffffff8, CX			
  proc.go:236		0x42bef9		483b6110		CMPQ 0x10(CX), SP			
  proc.go:236		0x42befd		7630			JBE 0x42bf2f				
  proc.go:236		0x42beff		4883ec18		SUBQ $0x18, SP				
  proc.go:236		0x42bf03		48896c2410		MOVQ BP, 0x10(SP)			
  proc.go:236		0x42bf08		488d6c2410		LEAQ 0x10(SP), BP			
  proc.go:237		0x42bf0d		c7042400000000		MOVL $0x0, 0(SP)			
  proc.go:237		0x42bf14		488d053dd32400		LEAQ 0x24d33d(IP), AX			
  proc.go:237		0x42bf1b		4889442408		MOVQ AX, 0x8(SP)			
  proc.go:237		0x42bf20		e8bb770000		CALL runtime.newproc(SB)		
  proc.go:238		0x42bf25		488b6c2410		MOVQ 0x10(SP), BP			
  proc.go:238		0x42bf2a		4883c418		ADDQ $0x18, SP				
  proc.go:238		0x42bf2e		c3			RET					
  proc.go:236		0x42bf2f		e8bc710200		CALL runtime.morestack_noctxt(SB)	
  proc.go:236		0x42bf34		ebba			JMP runtime.init.4(SB)			

TEXT runtime.init.5(SB) /usr/local/go/src/runtime/signal_unix.go
  signal_unix.go:64	0x43e320		c3			RET			

TEXT runtime.init(SB) <autogenerated>
  <autogenerated>:1	0x452a70		64488b0c25f8ffffff	MOVQ FS:0xfffffff8, CX			
  <autogenerated>:1	0x452a79		483b6110		CMPQ 0x10(CX), SP			
  <autogenerated>:1	0x452a7d		0f86e6000000		JBE 0x452b69				
  <autogenerated>:1	0x452a83		4883ec20		SUBQ $0x20, SP				
  <autogenerated>:1	0x452a87		48896c2418		MOVQ BP, 0x18(SP)			
  <autogenerated>:1	0x452a8c		488d6c2418		LEAQ 0x18(SP), BP			
  <autogenerated>:1	0x452a91		0fb60518423800		MOVZX runtime.initdone.(SB), AX		
  <autogenerated>:1	0x452a98		3c01			CMPL $0x1, AL				
  <autogenerated>:1	0x452a9a		760a			JBE 0x452aa6				
  <autogenerated>:1	0x452a9c		488b6c2418		MOVQ 0x18(SP), BP			
  <autogenerated>:1	0x452aa1		4883c420		ADDQ $0x20, SP				
  <autogenerated>:1	0x452aa5		c3			RET					
  <autogenerated>:1	0x452aa6		7507			JNE 0x452aaf				
  <autogenerated>:1	0x452aa8		e8a364fdff		CALL runtime.throwinit(SB)		
  <autogenerated>:1	0x452aad		0f0b			UD2					
  <autogenerated>:1	0x452aaf		c605fa41380001		MOVB $0x1, runtime.initdone.(SB)	
  float.go:9		0x452ab6		48b8000000000000f07f	MOVQ $0x7ff0000000000000, AX		
  float.go:9		0x452ac0		48890424		MOVQ AX, 0(SP)				
  float.go:9		0x452ac4		e86753fbff		CALL runtime.float64frombits(SB)	
  float.go:9		0x452ac9		f20f10442408		MOVSD_XMM 0x8(SP), X0			
  float.go:9		0x452acf		f20f1105f1423800	MOVSD_XMM X0, runtime.inf(SB)		
  select.go:48		0x452ad7		488d05c2cf1c00		LEAQ 0x1ccfc2(IP), AX			
  select.go:48		0x452ade		48890424		MOVQ AX, 0(SP)				
  select.go:48		0x452ae2		488d0507672200		LEAQ 0x226707(IP), AX			
  select.go:48		0x452ae9		4889442408		MOVQ AX, 0x8(SP)			
  select.go:48		0x452aee		e84d9ffdff		CALL runtime.funcPC(SB)			
  select.go:48		0x452af3		488b442410		MOVQ 0x10(SP), AX			
  select.go:48		0x452af8		48890599423800		MOVQ AX, runtime.chansendpc(SB)		
  select.go:49		0x452aff		488d053acf1c00		LEAQ 0x1ccf3a(IP), AX			
  select.go:49		0x452b06		48890424		MOVQ AX, 0(SP)				
  select.go:49		0x452b0a		488d05d7662200		LEAQ 0x2266d7(IP), AX			
  select.go:49		0x452b11		4889442408		MOVQ AX, 0x8(SP)			
  select.go:49		0x452b16		e8259ffdff		CALL runtime.funcPC(SB)			
  select.go:49		0x452b1b		488b442410		MOVQ 0x10(SP), AX			
  select.go:49		0x452b20		48890569423800		MOVQ AX, runtime.chanrecvpc(SB)		
  time.go:405		0x452b27		e854410000		CALL runtime.nanotime(SB)		
  time.go:405		0x452b2c		488b0424		MOVQ 0(SP), AX				
  time.go:405		0x452b30		48ffc8			DECQ AX					
  time.go:405		0x452b33		488905f6423800		MOVQ AX, runtime.startNano(SB)		
  <autogenerated>:1	0x452b3a		e84139fbff		CALL runtime.init.0(SB)			
  <autogenerated>:1	0x452b3f		e8ecdbfcff		CALL runtime.init.1(SB)			
  <autogenerated>:1	0x452b44		e85734fdff		CALL runtime.init.2(SB)			
  <autogenerated>:1	0x452b49		e8c267fdff		CALL runtime.init.3(SB)			
  <autogenerated>:1	0x452b4e		e89d93fdff		CALL runtime.init.4(SB)			
  <autogenerated>:1	0x452b53		e8c8b7feff		CALL runtime.init.5(SB)			
  <autogenerated>:1	0x452b58		c6055141380002		MOVB $0x2, runtime.initdone.(SB)	
  <autogenerated>:1	0x452b5f		488b6c2418		MOVQ 0x18(SP), BP			
  <autogenerated>:1	0x452b64		4883c420		ADDQ $0x20, SP				
  <autogenerated>:1	0x452b68		c3			RET					
  <autogenerated>:1	0x452b69		e882050000		CALL runtime.morestack_noctxt(SB)	
  <autogenerated>:1	0x452b6e		e9fdfeffff		JMP runtime.init(SB)	