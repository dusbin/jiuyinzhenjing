#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
//#include <stdio.h>
static void ** sys_call_table = NULL;
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
int __init test_init(void){
	printk("test_init\n");
	#ifdef __x86_64__
        printk("__x86_64__\n");
    #elif __i386__
	//#else
        printk("__i386__\n");
		sys_call_table = (void **)find_sys_call_table();
		printk("sys_call_tabl's address is 0x%lX\n",(unsigned long)sys_call_table);
    #endif
	return 0;
}
void __exit test_exit(void){
	printk("test_exit\n");
}
module_init(test_init);
module_exit(test_exit);
MODULE_LICENSE("GPL");

