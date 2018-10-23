#include<linux/slab.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/types.h>
#include<linux/errno.h>
#include<linux/fs.h>
#include<linux/mm.h>
#include<linux/cdev.h>
#include<asm/io.h>
#include<linux/sched.h>
#include<asm/uaccess.h>
#include<linux/kernel.h>
#include<linux/list.h>
#include<linux/sem.h>
#include<linux/pid.h>
 
 
#define MAJOR_NUM 260
#define MINOR_NUM 0
#define MINIOR_NUM 0
#define DEVICE_NUM 1
 
 
//以下为驱动需要实现的四个函数的生命，注册之后，用户态程序即调用此对应函数
static int firstdriver_open(struct inode *, struct file *);
static int firstdriver_release(struct inode *,struct file *);
static ssize_t firstdriver_read(struct file *, char __user *,size_t, loff_t *);
static ssize_t firstdriver_write(struct file *,char __user *,size_t ,loff_t *);
struct ret_type   //定义返回用户态的函数类型
{
        unsigned long start_code;
        unsigned long end_code;
};
 
static int firstdriver_major = MAJOR_NUM;  //定义主设备号
static int firstdriver_minor = MINOR_NUM;  //定义次设备号
struct file_operations firstdriver_fops =
        {
                .owner = THIS_MODULE,
                .open = firstdriver_open,
                .write = firstdriver_write,
                .release = firstdriver_release,
                .read = firstdriver_read,
        };
struct cdev *cdev;
static pid_t pidno = 1;
static int __init firstdriver_init(void)
{
                int ret = 0;
                dev_t devno = MKDEV(MAJOR_NUM,MINOR_NUM);
                cdev = cdev_alloc();//分配设备空间，这里为了简单，静态指定了设备号，最好动态申请
        if(register_chrdev_region(devno,DEVICE_NUM,"firstdriver"))
        {
                printk(KERN_ALERT"register_chrdev_region fail\n");
                return -1;
        }
        else
        {
                cdev_init(cdev,&firstdriver_fops);
                cdev->owner = THIS_MODULE;
                cdev->ops = &firstdriver_fops;
                if((ret = cdev_add(cdev,devno,1)))
                        printk(KERN_ALERT"Error in adding firstdriver\n");
                else
                        printk(KERN_ALERT"register success\n");
        }
        return ret;
}
static void __exit firstdriver_exit(void)
{
        dev_t devno = MKDEV(MAJOR_NUM,0);
        cdev_del(cdev);
        unregister_chrdev_region(devno,1);
        if(cdev)
                kfree(cdev);
        cdev = NULL;
}
static int firstdriver_open(struct inode * inode ,struct file * filp)
{
        printk(KERN_ALERT"open succedd\n");
        return 0;
}
static int firstdriver_release(struct inode* inode, struct file *filp)
{
        printk(KERN_ALERT"release success\n");
        return 0;
}
static ssize_t firstdriver_read(struct file *filp,char *buf,size_t len, loff_t * off)
{
        printk("READING:\n");
        struct task_struct *p = NULL;
        struct ret_type *tmp;
        struct pid *kpid = find_get_pid(pidno);
        tmp = kmalloc(sizeof(*tmp),GFP_KERNEL);
        if(tmp == NULL){
                return -ENOMEM;
        }
        p = pid_task(kpid,PIDTYPE_PID);
        if(p == NULL){
  kfree(tmp);
                printk(KERN_ALERT"find task failed\n");
                return -1;
        }
        if(p->mm == NULL){
                kfree(tmp);
                printk(KERN_ALERT"mm = NULL error\n");
                return -1;
        } else {
                tmp->start_code = p->mm->start_code;
                tmp->end_code = p->mm->end_code;
                printk(KERN_ALERT"start_code: %lu\n",tmp->start_code);
                printk(KERN_ALERT"end_code： %li\n",tmp->end_code);
        }
        if(copy_to_user(buf,tmp,sizeof(*tmp)))
                return -EFAULT;
        return sizeof(*tmp);
}
static ssize_t firstdriver_write(struct file *filp,char * buf,size_t len,loff_t * off)
{
        printk("writing:\n");
        if(copy_from_user(&pidno,buf,sizeof(int)))
                return -EFAULT;
        return sizeof(int);
}
module_init(firstdriver_init);
module_exit(firstdriver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("firstdriver");
