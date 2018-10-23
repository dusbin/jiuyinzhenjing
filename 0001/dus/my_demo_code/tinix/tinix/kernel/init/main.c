#define die()   __asm__("l1:\n\tjmp l1");

#include    <include/types.h>
#include    <include/stdarg.h>
#include    <include/task.h>
#include    <include/traps.h>
#include    <include/system.h>
#include    <include/io.h>
#include    <mm/memory.h>
#include    <dev/console.h>



// char printbuff[1024];

// static int printf(const char *fmt, ...)
// {
//     va_list args;
//     int i,j;
//     char c[2]={0,0};

//     va_start(args, fmt);
//     i = vsprintf(printbuff, fmt, args);
//     va_end(args);

//     for (j = 0; j < i; ++j)
//     {
//         c[0] = printbuff[j];
//         c[1] = 7;
//         write_con(c, 1);
//     }
//     return i;
// }

void k_main()
{
    //die();
    int sleep;
    int times=100;
    cmos_time time;
    cmos_date date;
    init_mem();
    init_con();
    init_task();
    init_traps();
    sti();
    
    while(1)
    {
        sleep = 1000000;
        cli();
        con_write("1\x07",2);
        sti();
        while (--sleep){};
    }
    die();
}

