#ifndef _FS_H
#define _FS_H

#include <include/types.h>
#include <include/limits.h>

#define F_M_SHARE               1               /* 只读，可共享 */
#define F_M_LOCK                2               /* 读写，不可共享 */

//文件操作跳转表结构
typedef struct _file_operations {
    s32 (*lseek) (u32);                         //定位文件当前的读写位置
    s32 (*read) (u8 *, u32);                    //读文件
    s32 (*write) (u8 *, u32);                   //写文件
}file_operations;

typedef struct _file {
    u8  f_mode;
    file_operations *f_op;                      //读写等操作文件的具体跳转指针
    u32 f_pos;                                  //文件的当前读写位置        
    u8  f_path[MAX_NAME];
}file;


#endif
