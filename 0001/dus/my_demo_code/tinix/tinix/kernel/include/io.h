/* 汇编操作 */


#ifndef _IO_H
#define _IO_H

#include <include/types.h>

extern void outb(char value, u16 port);
extern void outb_p(char value, u16 port);
extern u8 inb(u16 port);
extern u8 inb_p(u16 port);

extern u8 readcmos(int reg);
#define BCD_TO_BIN(val) ((val)=((val)&15)+((val)>>4)*10);
#endif
