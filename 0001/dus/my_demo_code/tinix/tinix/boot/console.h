/**/

#ifndef _CONSOLE_H
#define _CONSOLE_H
/*
#define dprint(format,args...) \
    printf("[%s-%s-L%d] "format, __FILE__, __FUNCTION__, __LINE__, ##args);
*/
#define COLUMNS                 80
#define LINES                   24
#define ATTR                    7
#define ATTR_OK                 10
#define ATTR_ERROR              12
#define VIDEO                   0xB8000

static void itoa (char *buf, int base, int d);
static void putchar (int c, int attr);

extern void dprint (int attr, const char *format, ...);
extern void cls (void);

#endif
/*
0 Black 黑色
1 Blue 蓝色
2 Green 绿色
3 Cyan 青色
4 Red 红色
5 Magenta 洋红
6 Brown 棕色
7 Light Gray 高亮灰色
8 Dark Gray 暗灰色
9 Light Blue 高亮蓝色
A Light Green 高亮绿色
B Light Cyan 高亮青色
C Light Red 高亮红色
D Light Magenta 高亮洋红
E Yellow 黄色
F White 白色
*/
