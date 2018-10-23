#ifndef _MEMDEV_H_
#define _MEMDEV_H_
#include<linux/ioctl.h>
#ifndef MEMDEV_MAJOR
#define MEMDEV_MAJOR 0
#endif
#ifndef MEMDEV_NR_DEVS
#define MEMDEV_NR_DEVS 2
#endif
#ifndef MEMDEV_SIZE
#define MEMDEV_SIZE 4096
#endif
struct mem_dev
{
    char *data;
    unsigned long size;
};
#define MEMDEV_IOC_MAGIC 'k'
#define MEMDEV_IOCPRINT _IO(MEMDEV_IOC_MAGIC,0)
#define MEMDEV_IOCGETDATA _IOR(MEMDEV_IOC_MAGIC,1,int)
#define MEMDEV_IOC_MAXNR 3
#endif
