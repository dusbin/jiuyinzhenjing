#ifndef _CONSOLE_H
#define _CONSOLE_H

#include <include/types.h>
#include <dev/fs.h>



extern u8 current_con;

extern void init_con();
extern s32 con_write(u8 *buff, u32 size);
extern s32 con_read(u8 *buff, u32 size);
extern s32 con_seek(u32 offset);

file_operations con_ops;

#endif /* _CONSOLE_H */

