#include <multiboot.h>
#include <console.h>

#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))
#define die()                   {while(1){}}

#define     MEM_SIZE            ((unsigned long *)0x90000)

extern unsigned char kernels[65535];     /* 内核开始位置 */
extern unsigned char kernele[65535];     /* 内核结束位置 */

static void load_kernel()
{
	unsigned long  ksize  =(unsigned long)(kernele-kernels);
	unsigned char* kaddr  =(unsigned char*)0;
	register unsigned long  i;

    dprint (ATTR_OK, "Kernel size %d bytes.", ksize);
	for(i=0;i<ksize;i++)   *kaddr++=kernels[i];            /*  copy    */
    
    for(i=0;i<ksize;i++)                                   /*  check   */
    {
        if (*(unsigned char *)i != kernels[i])
        {
            dprint (ATTR_ERROR, "Loading kernel error at 0x%x", i);
        }
    }
    dprint (ATTR_OK, "Kernel loading complete!");
}

static void save_bootinfo(multiboot_info_t *bootinfo)
{
    unsigned long test;
    unsigned char  org;

    if (CHECK_FLAG(bootinfo->flags, 0))
    {
        /* 保存内存信息 */
        *MEM_SIZE = (bootinfo->mem_upper * 1024) & 0xfffff000;
        if (*MEM_SIZE < 0x2000000)
        {
            dprint (ATTR_ERROR, 
                "Total available memory %u MB. Memory is too small.", 
                (*MEM_SIZE)/(1024*1024)-1);
            die();
        }
        dprint (ATTR_OK, "Total available memory %u MB.", (*MEM_SIZE)/(1024*1024)-1);
    }
}


void loader_main (unsigned long magic, multiboot_info_t *bootinfo)
{
    cls ();
    
    if (magic == MULTIBOOT_BOOTLOADER_MAGIC)
    {
        dprint (ATTR_OK, "Boot from grub!");
        save_bootinfo(bootinfo);
        dprint (ATTR_OK, "Loading kernel...");
        load_kernel();
        dprint (ATTR_OK, "Jump to kernel!");
    }
    else
    {
        dprint (ATTR_ERROR, "Invalid magic number: 0x%x", (unsigned) magic);
        dprint (ATTR_ERROR, "Only can boot from grub!");
        die();
    }
}
