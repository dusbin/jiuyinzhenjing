#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <asm/unistd.h>
//#include <stdio.h>
static void ** sys_call_table = NULL;
asmlinkage long (*orig_mkdir)(const char __user *pathname, int mode);
asmlinkage long xa_mkdir(const char __user *pathname, int mode);
unsigned int clear_cr0(void)
{
    unsigned int cr0 = 0;
    unsigned int ret;

    asm volatile ("movl %%cr0, %%eax"
              : "=a"(cr0)
              );
    ret = cr0;

    /* clear the 20 bit of CR0, a.k.a WP bit */
    cr0 &= 0xfffeffff;

    asm volatile ("movl %%eax, %%cr0"
              :
              : "a"(cr0)
              );
    return ret;
}
int syscall_check(){
	if(sys_call_table[__NR_mkdir] != xa_mkdir){
		printk("WARNING: sys_open has beed hacked!\n");
		return -1;
	}
	return 0;
}
asmlinkage long xa_mkdir(const char __user *pathname, int mode)
{
	if(syscall_check()!=0){
		printk("mould1 xa_mkdir is changed\n");
		return 0;
	}
	printk("mould1 pathname = %s\n",pathname);
	return orig_mkdir(pathname,mode);
}
unsigned long* find_sys_call_table(void)  
{  
    struct {  
        unsigned short  limit;  
        unsigned int    base;  
    } __attribute__ ( ( packed ) ) idtr;  
   
    struct {  
        unsigned short  offset_low;  
        unsigned short  segment_select;  
        unsigned char   reserved,   flags;  
        unsigned short  offset_high;  
    } __attribute__ ( ( packed ) ) * idt;

    unsigned long system_call = 0;        // x80中断处理程序system_call 地址  
    char *call_hex = "\xff\x14\x85";        // call 指令  
    char *code_ptr = NULL;
    char *p = NULL;
    unsigned long sct = 0x0;
    int i = 0;

    __asm__ ( "sidt %0": "=m" ( idtr ) );
    idt = ( void * ) ( idtr.base + 8 * 0x80 );
    system_call = ( idt->offset_high << 16 ) | idt->offset_low;

    code_ptr = (char *)system_call;
    for(i = 0;i < ( 100 - 2 ); i++) {
        if(code_ptr[i] == call_hex[0]&& code_ptr[i+1] == call_hex[1]&& code_ptr[i+2] == call_hex[2]){
            p = &code_ptr[i] + 3;
            break;
        }
    }
    if(p){
        sct = *(unsigned long*)p;
    }
	return (unsigned long*)sct;
} 
void restore_cr0(unsigned int val)
{
    asm volatile ("movl %%eax, %%cr0"
              :
              : "a"(val)
              );
}
int __init test_init(void){
	printk("test_init\n");
	unsigned int old_cr0;
	#ifdef __x86_64__
        printk("__x86_64__\n");
    #elif __i386__
	//#else
        printk("__i386__\n");
		sys_call_table = (void **)find_sys_call_table();
		printk("sys_call_tabl's address is 0x%lX\n",(unsigned long)sys_call_table);
		printk("uid = %d pid = %d\n",current->euid,current->pid);
    #endif
	orig_mkdir = sys_call_table[__NR_mkdir];
	old_cr0 = clear_cr0();
	sys_call_table[__NR_mkdir]= xa_mkdir;
	restore_cr0(old_cr0);
	return 0;
}
void __exit test_exit(void){
	unsigned int old_cr0;
	printk("test_exit\n");
	old_cr0 = clear_cr0();
	sys_call_table[__NR_mkdir] = orig_mkdir;
	restore_cr0(old_cr0);
}
module_init(test_init);
module_exit(test_exit);
MODULE_LICENSE("GPL");

