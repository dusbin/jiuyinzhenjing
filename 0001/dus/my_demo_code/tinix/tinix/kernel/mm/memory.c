
#include <include/types.h>
#include <mm/memory.h>

#define BOOT_MEMSIZE        ((u32 *)0xC0090000)

static u8 * mem_map = (u8*)PAGE_MAP;
static u32  page_max;
static u32  start_mm_index = 0;

/*
 *  内存管理初始化，仅在init/main.c中调用
 */
void init_mem()
{
    u32 index;
    page_max = (*BOOT_MEMSIZE)/PAGE_SIZE;               /* 物理内存页数 */
    start_mm_index += 0x102000/PAGE_SIZE;               /* 开始管理索引 + 内核占用页数 */
    start_mm_index += page_max/PAGE_SIZE + 1;           /* 开始管理索引 + mem_map 占用页数 */
    
    /* 初始化内存管理map */
    for (index = 0; index < page_max; index++)
    {
        if (index < start_mm_index) 
        {
            mem_map[index] = 1;                         /* 内核已使用内存标记 */
        }
        else
        {
            mem_map[index] = 0;                         /* 物理内存空闲标记 */
        }

    }

    /* 初始化pte 使用多少映射多少 */
    for (index = 0; index < PAGE_SIZE/4; index++)       /* PAGE_SIZE/4=pte地址个数 */
    {
        if (index < start_mm_index)
        {
            *(u32 *)(PAGE_PTE+index*4) = index*PAGE_SIZE | PAGE_RW | PAGE_PRESENT;
        }
        else
        {
            *(u32 *)(PAGE_PTE+index*4) = 0;
        }
    }

    *(u32 *)PAGE_DIR = 0;                               /* 取消低4m映射关系 */
    __flush_tlb();
}

