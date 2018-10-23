/*文件名: string.h
  说明:   提供一些内存处理函数
  作者:   Linus Torvalds
  日期:   2005/12/20
*/

#ifndef _LINUX_STRING_H_
#define _LINUX_STRING_H_

#include <include/types.h>

#ifndef NULL
#define NULL ((void *) 0)
#endif

/*
 * This string-include defines all string functions as 
 * functions. Use gcc. It also assumes ds=es=data space, this should be
 * normal. Most of the string-functions are rather heavily hand-optimized,
 * see especially strtok,strstr,str[c]spn. They should work, but are not
 * very easy to understand. Everything is done entirely within the register
 * set, making the functions fast and clean. String instructions have been
 * used through-out, making for "slightly" unclear code :-)
 *
 *      Copyright (C) 1991, 1992 Linus Torvalds
 */

extern  char * strcpy(char *dest, const char *src);

extern  char * strncpy(char *dest, const char *src, unsigned count);

extern  char * strcat(char *dest, const char *src);

extern  char * strncat(char *dest, const char *src, unsigned count);

extern  int strcmp(const char *cs, const char *ct);

extern  int strncmp(const char *cs, const char *ct, unsigned count);

extern  char * strchr(const char *s, char c);

extern  char * strrchr(const char *s, char c);

extern  unsigned strspn(const char *cs, const char *ct);

extern  unsigned strcspn(const char *cs, const char *ct);

extern  char * strpbrk(const char *cs, const char *ct);

extern  char * strstr(const char *cs, const char *ct);

extern  unsigned strlen(const char *s);

extern char *___strtok;

extern  char * strtok(char *s, const char *ct);

extern  void * memcpy(void *dest, const void *src, unsigned n);

extern  void * memmove(void *dest, const void *src, unsigned n);

extern  int memcmp(const void *cs, const void *ct, unsigned count);

extern  void * memchr(const void *cs, char c, unsigned count);

extern  void * memset(void *s, char c, unsigned count);

#endif

