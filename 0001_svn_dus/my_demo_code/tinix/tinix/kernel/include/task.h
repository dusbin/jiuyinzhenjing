/*
 * 任务管理，定时器管理
 */

#ifndef _TASK_H
#define _TASK_H

#include <include/types.h>
#include <include/limits.h>
#include <dev/fs.h>

/* 进程状态 */
#define TASK_NULL               0   /* 此位置没有任务 */
#define TASK_RUNNING            1   /* 运行 */
#define TASK_SLEEP              2   /* 睡眠 */
#define TASK_WAIT               3   /* IO等待 */
#define TASK_STOPED             4   /* 已终止 */

/* 定时器状态 */
#define TIMER_NULL              0   /* 无效定时器 */
#define TIMER_WAIT              1   /* 正在使用的定时器 */
#define TIMER_LOOP              2   /* 循环计数器 */

/* 寄存器结构 */
typedef struct _regs
{
    u32 eax;
    u32 ebx;
    u32 ecx;
    u32 edx;
    u32 esi;
    u32 edi;
    u32 ebp;
    u32 fs;
    u32 gs;
    u32 es;
    u32 ds;
    u32 err_code;    
    u32 kesp;                               /* 无特权级切换时，改它 */
    // u32 eip;
    // u32 cs;
    // u32 eflags;
    // u32 esp;                                /* 仅在特权切换时才出现 */
    // u32 ss;                                 /* 仅在特权切换时才出现 */
}__attribute__((packed)) regs;

/* 任务结构 */
typedef struct _task_struct
{
    u8  state;                              /* 进程状态 */
    u32 pid;                                /* 自身id */
    u32 father;                             /* 父进程id */
    s32 counter;                            /* 可使用时间片 */
    u32 user_time;                          /* 用户态运行时间(滴答数) */
    u32 system_time;                        /* 内核态运行时间(滴答数) */
    u32 start_time;                         /* 进程开始的时间 */
    u8  cpl;                                /* 被调度前的cpl */

    s8  con;                                /* 进程使用的控制台，通常继承自父进程，负数则没有 */
    u32 fid[MAX_OPEN];                      /* 进程使用的文件表,0号是执行文件本身，1号文件是con */

    regs t_regs;
    u32 pgd;                                /* 进程页目录 */
} __attribute__((packed)) task_struct;

typedef void (*dotimefun_ptr)(s32 handle);  /* 定时器回调 原型 */


/* 定时器的结构描述 */
typedef struct _timer_struct
{
    u8  state;
    u32 counter;
    dotimefun_ptr timer_dirver;
}__attribute__((packed)) timer_struct;

/* 时间结构 */
typedef struct  {
    unsigned char sec, min, hour;
}__attribute__((packed)) cmos_time;

/* 日期结构 */
typedef struct  {
    int dayofweek, day, month, year;
}__attribute__((packed)) cmos_date;



extern task_struct  task_list[MAX_TASK];        /* 任务队列 */
extern timer_struct timer_list[MAX_TIMER];      /* 定时器队列 */
extern u32  current;                            /* 当前任务 */
extern u32  jiffies;                            /* 开机后的滴答数 */
extern u32  last_pid;

extern void init_task();
extern void time_hander();
//extern u32  add_task();
//extern u32  del_task(u32);
#endif
