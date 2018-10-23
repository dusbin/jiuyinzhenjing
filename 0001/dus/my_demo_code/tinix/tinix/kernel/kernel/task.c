#include <include/types.h>
#include <include/task.h>
#include <include/limits.h>
#include <include/string.h>
#include <include/system.h>
#include <include/stdarg.h>
#include <include/io.h>
#include <dev/console.h>
#include <mm/memory.h>

#define _PUSH(esp, value) do{ \
    (esp)-=sizeof(u32); \
    *(u32*)(esp)=value; \
    } while(0)


task_struct  task_list[MAX_TASK];        /* 任务队列 */
timer_struct timer_list[MAX_TIMER];      /* 定时器队列 */
regs tmp;
u32  current=0;                          /* 当前任务 */
u32  jiffies=0;                          /* 开机后的滴答数 */
u32  last_task=0;

void scheduler(u8 cpl, regs *pregs)
{
    u32 next;

    /* 先给时间片 */
    task_list[current].counter = 1;                 

    /* 遍历任务队列,找个可以调度的任务 */
    if (current == MAX_TASK-1) next=0;
    else next = current+1;
    while (1)                                       
    {
        if (task_list[next].counter > 0 && task_list[next].state == TASK_RUNNING)
        break;
        if (next == MAX_TASK-1) next=0;
        else next++;
    }

    /* 修改陷阱帧 */
    task_list[current].cpl = cpl;
    memcpy(&task_list[current].t_regs, pregs, sizeof(regs));
    memcpy(pregs, &task_list[next].t_regs, sizeof(regs));

    current = next;
}

void timer_handler(u8 cpl, regs *pregs)
{
    ++jiffies;
    
    if (3 == cpl) {                         //对进程进行计时
        task_list[current].user_time++;     //用户态的执行时间
    } else {
        task_list[current].system_time++;   //内核态的执行时间
    }    

    //spring_timer();                         //触发定时器
    
    if ((--task_list[current].counter) == 0) 
    {                                       //如果时间片被用完则需要进行调度 
        scheduler(cpl, pregs);
        return;
        //need_resched = 1;   
    } else {                                //如果时间片没有用完则继续执行
        return;
    }
    
    if (cpl == 3) {                         //如果进程在用户态下被中断
        //scheduler();                        //进行调度
    }
}

void testfun()
{
    int sleep=1000000;
    while(1)
    {
        sleep=1000000;
        cli();
        con_write("2\x06",2);
        sti();
        while (--sleep){};
    }
}
/* 初始化任务队列，定时器队列，系统滴答数 */
void init_task()
{
    int i;
    for (i = 0; i < MAX_TIMER; i++)
    {
        timer_list[i].state = TIMER_NULL;       /* 将所有定时器置为空 */
    }
    for (i = 0; i < MAX_TASK; i++)
    {
        task_list[i].state = TASK_NULL;         /* 将任务队列中所有项置为空 */
    }

    current = 0;                                /* 捏造一个0号任务 */
    task_list[0].state = TASK_RUNNING;            
    task_list[0].counter = 1;                 /* 其实不需要时间片，因为没有出现第二个任务 */
    task_list[0].father = 0;
    task_list[0].user_time = 0;
    task_list[0].system_time = 0;
    task_list[0].start_time = 0;
    task_list[0].pgd = PAGE_DIR;

    jiffies = 0;                                /* 滴答声初始化 */

    task_list[1].state = TASK_RUNNING;            
    task_list[1].counter = 1;                 /* 其实不需要时间片，因为没有出现第二个任务 */
    task_list[1].father = 0;
    task_list[1].user_time = 0;
    task_list[1].system_time = 0;
    task_list[1].start_time = 0;
    task_list[1].pgd = PAGE_DIR;
    memset(&(task_list[1].t_regs),0,sizeof(regs));
    task_list[1].t_regs.ds = 0x10;
    task_list[1].t_regs.gs = 0x10;
    task_list[1].t_regs.es = 0x10;
    task_list[1].t_regs.fs = 0x10;
    task_list[1].t_regs.kesp = 0xC0070000;

    _PUSH(task_list[1].t_regs.kesp, 0x200);
    _PUSH(task_list[1].t_regs.kesp, 0x08);
    _PUSH(task_list[1].t_regs.kesp, testfun);

    //set_system_gate(0x80,&system_call);
}
