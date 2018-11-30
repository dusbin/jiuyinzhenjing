#include <linux/module.h>
#include <linux/kernel.h>  
#include <linux/init.h>  
#include <linux/syscalls.h>
#include <linux/sched.h>  
#include <asm/unistd.h>
#include <linux/fs.h>
//#include "stdlib.h"
//#include "stdio.h"

//#define FILE_NAME "/opt/uxsino/cdps/agent/etc/addr_sys_call_table"

MODULE_LICENSE("GPL");
//extern void * sys_call_table[];
static unsigned long temp = 0;
int orig_cr0;
void** sys_call_tables;
static int clear_cr0(void);
static void setback_cr0(int val);

static int clear_cr0(void)
{
    unsigned int cr0 = 0;
    unsigned int ret;
    asm volatile ("movq %%cr0, %%rax":"=a"(cr0));
    ret = cr0;
    cr0 &=0xfffeffff;
    asm volatile ("movq %%rax, %%cr0": :"a"(cr0));
    return ret;
}
static void setback_cr0(int val)
{
    asm volatile ("movq  %%rax,  %%cr0": :"a"(val));
}
//asmlinkage long (*orig_mkdir)(const char*, int);
asmlinkage long (*orig_mkdir)(const char __user *pathname, int mode);
asmlinkage ssize_t  (*orig_read)  (unsigned int fd, char * buf, size_t count) = NULL; 
asmlinkage ssize_t  (*orig_write) (unsigned int fd, char * buf, size_t count);
int read_file(unsigned long *addr)
{
	char sys_call_addr[17];
/*	struct file *fp = NULL;
	ssize_t ret;
	mm_segment_t fs;
	loff_t pos;
	memset(sys_call_addr,0x00,sizeof(sys_call_addr));
	fp = filp_open(FILE_NAME,O_RDWR |O_CREAT,0644);
	if(IS_ERR(fp)){
		printk("create file error\n");
		return -1;
	}
	fs = get_fs();
	set_fs(fs);
	pos = 0;
	ret = fp->f_op->read(fp,sys_call_addr,sizeof(sys_call_addr),&pos);
	if(ret > 0){*/
	//FILE *fp;
	//if(NULL==(fp=popen("cat /boot/System.map-3.13.0-32-generic |grep \" sys_call_table\"|awk '{printf $1}'","r"))){
	//	exit(1);
	//}
	//fgets(sys_call_addr,sizeof(sys_call_addr),fp);
	//pcolse(fp);
	//fp = NULL;
		sprintf(sys_call_addr,"%s","ffffffff816004a0");
		printk("0x%s\n",sys_call_addr);
		*addr = simple_strtol(sys_call_addr,NULL,16);
		printk("%ld\n",*addr);
/*	}else{
		printk("open file err !\n");
	}
	filp_close(fp,NULL);
*/
	return 0;
}
asmlinkage long hacked_mkdir(const char *pathname,int mode)
{
	printk("PID %d called sys_mkdir \n",current->pid);
	printk("pathname : %s,%d\n",pathname,mode);
	//return 0;    //lanjie
	return orig_mkdir(pathname,mode);
}
asmlinkage ssize_t  hacked_write(unsigned int fd, char * buf, size_t count)
{
	ssize_t rt;
	//printk("fd:%u,buf:%s,count:%zu\n",fd,buf,count);
    rt = orig_write(fd,buf,count);
    return rt;
}
asmlinkage ssize_t  hacked_read(unsigned int fd, char * buf, size_t count)
{
	//printk("fd:%u,buf:%s,count:%zu\n",fd,buf,count);
    return orig_read(fd,buf,count);
} 
static int hello_init(void)
{

	unsigned long addr;
	int ret = 0;
	ret = read_file(&addr);
	if(ret != 0){
		printk("read_file error \n");
	}
	printk("hello world\n");
	temp = addr;
	printk("temp :%ld\n",temp);
	sys_call_tables = (void **)temp;
	printk("sys_call_tables :%p\n",*sys_call_tables);
	orig_mkdir = sys_call_tables[__NR_mkdir];
	orig_read  = sys_call_tables[__NR_read];
	orig_write = sys_call_tables[__NR_write];
	
	orig_cr0= clear_cr0();
	
	printk("baocun orig_mkdir .....%p\n",orig_mkdir);	
	sys_call_tables[__NR_mkdir] = hacked_mkdir;
	sys_call_tables[__NR_read]  = hacked_read;
	sys_call_tables[__NR_write] = hacked_write;
	setback_cr0(orig_cr0);
	printk("tihuan orig_mkdir .....\n");	
	return 0;
}
static void hello_exit(void)
{
	printk("Goodbye world\n");
	orig_cr0= clear_cr0();
	sys_call_tables[__NR_mkdir] = orig_mkdir;
	sys_call_tables[__NR_read]  = orig_read;
	sys_call_tables[__NR_write] = orig_write;
	setback_cr0(orig_cr0);
}
module_init(hello_init);
module_exit(hello_exit);
