/* 汇编操作 */

#include <include/types.h>
#include <include/io.h>


void inline outb(char value, u16 port)
{
    __asm__ __volatile__ ("outb %0,%1"
            ::"a" ((char) value), "d" ((u16) port));
}

void inline outb_p(char value, u16 port)
{
    __asm__ __volatile__ ("outb %0,%1\n\t"
            "outb %0,$0x80\n\t"
            "outb %0,$0x80\n\t"
            "outb %0,$0x80\n\t"
            "outb %0,$0x80"
            ::"a" ((char) value),"d" ((u16) port));
}

u8 inline inb(u16 port)
{
    u8 _v;
    __asm__ __volatile__ ("inb %1,%0"
            :"=a" (_v)
            :"d" ((unsigned short) port));
    return _v;
}

u8 inline inb_p(u16 port)
{
    u8 _v;
    __asm__ __volatile__ ("inb %1, %0\n\t"
            "outb %0,$0x80\n\t"
            "outb %0,$0x80\n\t"
            "outb %0,$0x80\n\t"
            "outb %0,$0x80"
            :"=a" (_v)
            :"d" ((u16) port));
    return _v;
}

u8 inline readcmos(int reg)
{
	outb(reg,0x70);
	return (unsigned char) inb(0x71);
}
