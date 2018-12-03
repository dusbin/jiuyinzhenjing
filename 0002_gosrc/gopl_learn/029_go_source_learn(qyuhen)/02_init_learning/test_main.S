TEXT main.init.0(SB) /mnt/hgfs/github/jiuyinzhenjing/0002_gosrc/gopl_learn/029_go_source_learn(qyuhen)/02_init_learning/example.go
  example.go:6		0x5e3a90		64488b0c25f8ffffff	MOVQ FS:0xfffffff8, CX			
  example.go:6		0x5e3a99		483b6110		CMPQ 0x10(CX), SP			
  example.go:6		0x5e3a9d		767f			JBE 0x5e3b1e				
  example.go:6		0x5e3a9f		4883ec68		SUBQ $0x68, SP				
  example.go:6		0x5e3aa3		48896c2460		MOVQ BP, 0x60(SP)			
  example.go:6		0x5e3aa8		488d6c2460		LEAQ 0x60(SP), BP			
  example.go:7		0x5e3aad		0f57c0			XORPS X0, X0				
  example.go:7		0x5e3ab0		0f11442438		MOVUPS X0, 0x38(SP)			
  example.go:7		0x5e3ab5		488d442438		LEAQ 0x38(SP), AX			
  example.go:7		0x5e3aba		4889442430		MOVQ AX, 0x30(SP)			
  example.go:7		0x5e3abf		8400			TESTB AL, 0(AX)				
  example.go:7		0x5e3ac1		488d0578900200		LEAQ 0x29078(IP), AX			
  example.go:7		0x5e3ac8		4889442438		MOVQ AX, 0x38(SP)			
  example.go:7		0x5e3acd		488d052c090b00		LEAQ main.statictmp_0(SB), AX		
  example.go:7		0x5e3ad4		4889442440		MOVQ AX, 0x40(SP)			
  example.go:7		0x5e3ad9		488b442430		MOVQ 0x30(SP), AX			
  example.go:7		0x5e3ade		8400			TESTB AL, 0(AX)				
  example.go:7		0x5e3ae0		eb00			JMP 0x5e3ae2				
  example.go:7		0x5e3ae2		4889442448		MOVQ AX, 0x48(SP)			
  example.go:7		0x5e3ae7		48c744245001000000	MOVQ $0x1, 0x50(SP)			
  example.go:7		0x5e3af0		48c744245801000000	MOVQ $0x1, 0x58(SP)			
  example.go:7		0x5e3af9		48890424		MOVQ AX, 0(SP)				
  example.go:7		0x5e3afd		48c744240801000000	MOVQ $0x1, 0x8(SP)			
  example.go:7		0x5e3b06		48c744241001000000	MOVQ $0x1, 0x10(SP)			
  example.go:7		0x5e3b0f		e86cfbebff		CALL fmt.Println(SB)			
  example.go:8		0x5e3b14		488b6c2460		MOVQ 0x60(SP), BP			
  example.go:8		0x5e3b19		4883c468		ADDQ $0x68, SP				
  example.go:8		0x5e3b1d		c3			RET					
  example.go:6		0x5e3b1e		e8cdf5e6ff		CALL runtime.morestack_noctxt(SB)	
  example.go:6		0x5e3b23		e968ffffff		JMP main.init.0(SB)			

TEXT main.init.1(SB) /mnt/hgfs/github/jiuyinzhenjing/0002_gosrc/gopl_learn/029_go_source_learn(qyuhen)/02_init_learning/example.go
  example.go:12		0x5e3b60		64488b0c25f8ffffff	MOVQ FS:0xfffffff8, CX			
  example.go:12		0x5e3b69		483b6110		CMPQ 0x10(CX), SP			
  example.go:12		0x5e3b6d		767f			JBE 0x5e3bee				
  example.go:12		0x5e3b6f		4883ec68		SUBQ $0x68, SP				
  example.go:12		0x5e3b73		48896c2460		MOVQ BP, 0x60(SP)			
  example.go:12		0x5e3b78		488d6c2460		LEAQ 0x60(SP), BP			
  example.go:13		0x5e3b7d		0f57c0			XORPS X0, X0				
  example.go:13		0x5e3b80		0f11442438		MOVUPS X0, 0x38(SP)			
  example.go:13		0x5e3b85		488d442438		LEAQ 0x38(SP), AX			
  example.go:13		0x5e3b8a		4889442430		MOVQ AX, 0x30(SP)			
  example.go:13		0x5e3b8f		8400			TESTB AL, 0(AX)				
  example.go:13		0x5e3b91		488d05a88f0200		LEAQ 0x28fa8(IP), AX			
  example.go:13		0x5e3b98		4889442438		MOVQ AX, 0x38(SP)			
  example.go:13		0x5e3b9d		488d056c080b00		LEAQ main.statictmp_1(SB), AX		
  example.go:13		0x5e3ba4		4889442440		MOVQ AX, 0x40(SP)			
  example.go:13		0x5e3ba9		488b442430		MOVQ 0x30(SP), AX			
  example.go:13		0x5e3bae		8400			TESTB AL, 0(AX)				
  example.go:13		0x5e3bb0		eb00			JMP 0x5e3bb2				
  example.go:13		0x5e3bb2		4889442448		MOVQ AX, 0x48(SP)			
  example.go:13		0x5e3bb7		48c744245001000000	MOVQ $0x1, 0x50(SP)			
  example.go:13		0x5e3bc0		48c744245801000000	MOVQ $0x1, 0x58(SP)			
  example.go:13		0x5e3bc9		48890424		MOVQ AX, 0(SP)				
  example.go:13		0x5e3bcd		48c744240801000000	MOVQ $0x1, 0x8(SP)			
  example.go:13		0x5e3bd6		48c744241001000000	MOVQ $0x1, 0x10(SP)			
  example.go:13		0x5e3bdf		e89cfaebff		CALL fmt.Println(SB)			
  example.go:14		0x5e3be4		488b6c2460		MOVQ 0x60(SP), BP			
  example.go:14		0x5e3be9		4883c468		ADDQ $0x68, SP				
  example.go:14		0x5e3bed		c3			RET					
  example.go:12		0x5e3bee		e8fdf4e6ff		CALL runtime.morestack_noctxt(SB)	
  example.go:12		0x5e3bf3		e968ffffff		JMP main.init.1(SB)			

TEXT main.init.2(SB) /mnt/hgfs/github/jiuyinzhenjing/0002_gosrc/gopl_learn/029_go_source_learn(qyuhen)/02_init_learning/test.go
  test.go:7		0x5e3c00		64488b0c25f8ffffff	MOVQ FS:0xfffffff8, CX			
  test.go:7		0x5e3c09		483b6110		CMPQ 0x10(CX), SP			
  test.go:7		0x5e3c0d		767f			JBE 0x5e3c8e				
  test.go:7		0x5e3c0f		4883ec68		SUBQ $0x68, SP				
  test.go:7		0x5e3c13		48896c2460		MOVQ BP, 0x60(SP)			
  test.go:7		0x5e3c18		488d6c2460		LEAQ 0x60(SP), BP			
  test.go:8		0x5e3c1d		0f57c0			XORPS X0, X0				
  test.go:8		0x5e3c20		0f11442438		MOVUPS X0, 0x38(SP)			
  test.go:8		0x5e3c25		488d442438		LEAQ 0x38(SP), AX			
  test.go:8		0x5e3c2a		4889442430		MOVQ AX, 0x30(SP)			
  test.go:8		0x5e3c2f		8400			TESTB AL, 0(AX)				
  test.go:8		0x5e3c31		488d05088f0200		LEAQ 0x28f08(IP), AX			
  test.go:8		0x5e3c38		4889442438		MOVQ AX, 0x38(SP)			
  test.go:8		0x5e3c3d		488d05dc070b00		LEAQ main.statictmp_2(SB), AX		
  test.go:8		0x5e3c44		4889442440		MOVQ AX, 0x40(SP)			
  test.go:8		0x5e3c49		488b442430		MOVQ 0x30(SP), AX			
  test.go:8		0x5e3c4e		8400			TESTB AL, 0(AX)				
  test.go:8		0x5e3c50		eb00			JMP 0x5e3c52				
  test.go:8		0x5e3c52		4889442448		MOVQ AX, 0x48(SP)			
  test.go:8		0x5e3c57		48c744245001000000	MOVQ $0x1, 0x50(SP)			
  test.go:8		0x5e3c60		48c744245801000000	MOVQ $0x1, 0x58(SP)			
  test.go:8		0x5e3c69		48890424		MOVQ AX, 0(SP)				
  test.go:8		0x5e3c6d		48c744240801000000	MOVQ $0x1, 0x8(SP)			
  test.go:8		0x5e3c76		48c744241001000000	MOVQ $0x1, 0x10(SP)			
  test.go:8		0x5e3c7f		e8fcf9ebff		CALL fmt.Println(SB)			
  test.go:9		0x5e3c84		488b6c2460		MOVQ 0x60(SP), BP			
  test.go:9		0x5e3c89		4883c468		ADDQ $0x68, SP				
  test.go:9		0x5e3c8d		c3			RET					
  test.go:7		0x5e3c8e		e85df4e6ff		CALL runtime.morestack_noctxt(SB)	
  test.go:7		0x5e3c93		e968ffffff		JMP main.init.2(SB)			

TEXT main.init(SB) <autogenerated>
  <autogenerated>:1	0x5e3e10		64488b0c25f8ffffff	MOVQ FS:0xfffffff8, CX							
  <autogenerated>:1	0x5e3e19		483b6110		CMPQ 0x10(CX), SP							
  <autogenerated>:1	0x5e3e1d		766b			JBE 0x5e3e8a								
  <autogenerated>:1	0x5e3e1f		4883ec08		SUBQ $0x8, SP								
  <autogenerated>:1	0x5e3e23		48892c24		MOVQ BP, 0(SP)								
  <autogenerated>:1	0x5e3e27		488d2c24		LEAQ 0(SP), BP								
  <autogenerated>:1	0x5e3e2b		0fb6055c2e1f00		MOVZX main.initdone.(SB), AX						
  <autogenerated>:1	0x5e3e32		3c01			CMPL $0x1, AL								
  <autogenerated>:1	0x5e3e34		7702			JA 0x5e3e38								
  <autogenerated>:1	0x5e3e36		eb09			JMP 0x5e3e41								
  <autogenerated>:1	0x5e3e38		488b2c24		MOVQ 0(SP), BP								
  <autogenerated>:1	0x5e3e3c		4883c408		ADDQ $0x8, SP								
  <autogenerated>:1	0x5e3e40		c3			RET									
  <autogenerated>:1	0x5e3e41		0fb605462e1f00		MOVZX main.initdone.(SB), AX						
  <autogenerated>:1	0x5e3e48		3c01			CMPL $0x1, AL								
  <autogenerated>:1	0x5e3e4a		7402			JE 0x5e3e4e								
  <autogenerated>:1	0x5e3e4c		eb07			JMP 0x5e3e55								
  <autogenerated>:1	0x5e3e4e		e8fd50e4ff		CALL runtime.throwinit(SB)						
  <autogenerated>:1	0x5e3e53		0f0b			UD2									
  <autogenerated>:1	0x5e3e55		c605322e1f0001		MOVB $0x1, main.initdone.(SB)						
  <autogenerated>:1	0x5e3e5c		e81f73ecff		CALL fmt.init(SB)							
  <autogenerated>:1	0x5e3e61		e89a61ffff		CALL net/http.init(SB)							
  <autogenerated>:1	0x5e3e66		e8c5fbffff		CALL _/mnt/hgfs/github/jiuyinzhenjing/0002_gosrc/gopl_learn/029_go_source_learn_qyuhen_/02_init_learning/lib.init(SB)	
  <autogenerated>:1	0x5e3e6b		e820fcffff		CALL main.init.0(SB)							
  <autogenerated>:1	0x5e3e70		e8ebfcffff		CALL main.init.1(SB)							
  <autogenerated>:1	0x5e3e75		e886fdffff		CALL main.init.2(SB)							
  <autogenerated>:1	0x5e3e7a		c6050d2e1f0002		MOVB $0x2, main.initdone.(SB)						
  <autogenerated>:1	0x5e3e81		488b2c24		MOVQ 0(SP), BP								
  <autogenerated>:1	0x5e3e85		4883c408		ADDQ $0x8, SP								
  <autogenerated>:1	0x5e3e89		c3			RET									
  <autogenerated>:1	0x5e3e8a		e861f2e6ff		CALL runtime.morestack_noctxt(SB)					
  <autogenerated>:1	0x5e3e8f		e97cffffff		JMP main.init(SB)			