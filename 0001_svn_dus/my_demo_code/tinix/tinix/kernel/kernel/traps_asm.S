/*
 *  linux/kernel/asm.s
 *
 *  (C) 1991  Linus Torvalds
 */

/*
 * asm.s contains the low-level code for most hardware faults.
 * page_exception is handled by the mm, so that isn't here. This
 * file also handles (hopefully) fpu-exceptions due to TS-bit, as
 * the fpu must be properly saved/resored. This hasn't been tested.
 */

.globl _divide_error,_debug,_nmi,_int3,_overflow,_bounds,_invalid_op
.globl _double_fault,_coprocessor_segment_overrun
.globl _invalid_TSS,_segment_not_present,_stack_segment
.globl _general_protection,_coprocessor_error,_reserved
.globl _device_not_available,_page_fault,_parallel_interrupt

.globl _timer_interrupt

/**************************************/
/*          无错误码处理流程          */
/**************************************/
no_error_code:
    xchgl %eax,(%esp)
    pushl %ebx
    pushl %ecx
    pushl %edx
    pushl %edi
    pushl %esi
    pushl %ebp
    push %ds
    push %es
    push %fs
    pushl $0        # "error code"
    lea 44(%esp),%edx
    pushl %edx
    movl $0x10,%edx
    mov %dx,%ds
    mov %dx,%es
    mov %dx,%fs
    call *%eax
    addl $8,%esp
    pop %fs
    pop %es
    pop %ds
    popl %ebp
    popl %esi
    popl %edi
    popl %edx
    popl %ecx
    popl %ebx
    popl %eax
    iret

.align  4
_divide_error:
    pushl $_do_divide_error
    jmp no_error_code

.align  4
_debug:
    pushl $_do_int3     # __do_debug
    jmp no_error_code

.align  4
_nmi:
    pushl $_do_nmi
    jmp no_error_code

.align  4
_int3:
    pushl $_do_int3
    jmp no_error_code

.align  4
_overflow:
    pushl $_do_overflow
    jmp no_error_code

.align  4
_bounds:
    pushl $_do_bounds
    jmp no_error_code

.align  4
_invalid_op:
    pushl $_do_invalid_op
    jmp no_error_code

.align  4
_coprocessor_segment_overrun:
    pushl $_do_coprocessor_segment_overrun
    jmp no_error_code

.align  4
_device_not_available:
    pushl $_do_device_not_available
    jmp no_error_code

.align  4
_coprocessor_error:
    pushl $_do_coprocessor_error
    jmp no_error_code

.align  4
_reserved:
    pushl $_do_reserved
    jmp no_error_code


.align  4
_parallel_interrupt:
    pushl %eax
    movb $0x20,%al
    outb %al,$0x20
    popl %eax
    iret


/**************************************/
/*          有错误码处理流程          */
/**************************************/
error_code:
    xchgl %eax,4(%esp)      # error code <-> %eax
    xchgl %ebx,(%esp)       # &function <-> %ebx
    pushl %ecx
    pushl %edx
    pushl %edi
    pushl %esi
    pushl %ebp
    push %ds
    push %es
    push %fs
    pushl %eax          # error code
    lea 44(%esp),%eax       # offset
    pushl %eax
    movl $0x10,%eax
    mov %ax,%ds
    mov %ax,%es
    mov %ax,%fs
    call *%ebx
    addl $8,%esp
    pop %fs
    pop %es
    pop %ds
    popl %ebp
    popl %esi
    popl %edi
    popl %edx
    popl %ecx
    popl %ebx
    popl %eax
    iret

.align  4
_double_fault:
    pushl $_do_double_fault
    jmp error_code

.align  4
_invalid_TSS:
    pushl $_do_invalid_TSS
    jmp error_code

.align  4
_segment_not_present:
    pushl $_do_segment_not_present
    jmp error_code

.align  4
_stack_segment:
    pushl $_do_stack_segment
    jmp error_code

.align  4
_general_protection:
    pushl $_do_general_protection
    jmp error_code

.align  4
_page_fault:
    pushl $_do_page_fault
    jmp error_code

.align  4
_timer_interrupt:
    pushl %esp
    pushl $0                            /* error_code */
    push %ds
    push %es                            # 保存段寄存器
    push %gs
    push %fs
    pushl %ebp
    pushl %edi
    pushl %esi
    pushl %edx 
    pushl %ecx
    pushl %ebx 
    pushl %eax 

    movl  56(%esp), %eax                # 获取被中断的cs
    
    pushl %esp
    mov   $0x10, %bx                    #
    mov   %bx, %ds                      #
    mov   %bx, %es                      # 设置数据段寄存器的值
    andl  $3, %eax                      # 获取dpl
    pushl %eax                          #     
    call  _timer_handler                # 调用时钟中断处理代码
    addl  $8,%esp

    movb $0x20,%al                      # EOI to interrupt controller #1
    outb %al,$0x20

    popl  %eax                          #恢复系统调用后的返回值 
    popl  %ebx 
    popl  %ecx    
    popl  %edx
    popl  %esi
    popl  %edi
    popl  %ebp    
    pop   %fs
    pop   %gs
    pop   %es
    pop   %ds
    addl  $4,%esp
    popl  %esp
    iret