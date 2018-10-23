/*
 * 陷阱
 */

#ifndef _TRAP_H
#define _TRAP_H

#include <include/types.h>

#define HZ 100
#define LATCH (1193180 / HZ)

 
extern void init_trap();

#endif