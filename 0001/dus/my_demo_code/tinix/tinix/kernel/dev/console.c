
#include <include/types.h>
#include <include/io.h>
#include <include/system.h>
#include <include/stdarg.h>
#include <include/string.h>
#include <dev/console.h>

/*
0,0 |-----------x
    |
    |
    |
    y
*/
static  u32 x,y;                            /* 光标位置 */
static  u32 COLUMNS = 80;                   /* 行字符数 */
static  u32 LINES   = 25;                   /* 每屏行数 */
static  u32 show_cursor = 1;                /* 是否显示光标 */
static  u16 eraser;                         /* 橡皮字符 */
#define POS     ((y*COLUMNS+x)*2)           /* 光标在屏幕缓冲区中的偏移 */
#define VSIZE   (video_end - video_base)    /* 屏幕缓冲区大小 */

static  u32 video_base     = 0xC00B8000;
static  u32 video_end      = 0xC00B8FA0;
static  u16 video_port_reg = 0x3d4;         /* 显卡寄存器选择端口   */
static  u16 video_port_val = 0x3d5;         /* 显卡寄存器写值端口   */

void goto_xy(int new_x, int new_y)
{
    if (new_x > COLUMNS || new_x < 0) x=0;
    else x = new_x;
    if (new_y > COLUMNS || new_y < 0) y=0;
    else y = new_y;
}

static void newline()
{
    if (y < LINES-1)    /* 如果屏幕未满，则移到下一行 */
    {
        y++;
        x = 0;
        return;
    }
    /* 否则，第一行到倒数第一行向上滚动，最后一行清除 */
    memmove((void *)video_base, (void *)(video_base + COLUMNS*2), VSIZE - COLUMNS*2);
    memset((void *)(video_base + VSIZE - COLUMNS*2), 0, COLUMNS*2);
    x = 0;
    return;
}

/* 更新光标 */
static void set_cursor()
{
    //cli();
    if (show_cursor) 
    {
        /* 显示光标相当于将光标位置设置到屏幕区以内 */
        outb_p(14, video_port_reg);
        outb_p(0xff & ((POS) >> 9), video_port_val);
        outb_p(15, video_port_reg);
        outb_p(0xff & ((POS) >> 1), video_port_val);
    }
    else 
    {
        /* 隐藏光标相当于将光标位置设置到屏幕区以外，VSIZE即屏幕缓冲区大小 */
        outb_p(14, video_port_reg);
        outb_p(0xff & ((VSIZE) >> 9), video_port_val);
        outb_p(15, video_port_reg);
        outb_p(0xff & ((VSIZE) >> 1), video_port_val);
    }
    //sti();
}
/* 控制台写操作,buff带颜色 */
s32 con_write(u8 *buff, u32 size)
{
    int index;
    if (POS >= VSIZE)
    {
        x = 0;
        y = 0;
    }
    for (index = 0; index < size; index+=2)
    {
        if (buff[index] == '\r')    {x = 0;continue;}
        if (buff[index] == '\n')    {newline();continue;}
        *(u8 *)(video_base + POS) = buff[index];
        *(u8 *)(video_base + POS + 1) = buff[index+1];
        x++;
    }
}

/* 控制台读操作 */
s32 con_read(u8* buff, u32 size)
{
}
/* 初始化控制台 */
void init_con()
{
    x = 0;
    y = 0;
    con_ops.lseek=con_seek;
    con_ops.read=con_read;
    con_ops.write=con_write;
    
}

s32 con_seek(u32 offset)
{

}
