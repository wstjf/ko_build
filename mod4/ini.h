#ifndef CONFIG_INI
#define CONFIG_INI


#define MAX_THREAD 3
// #define CFI_NEW_KERNEL

//#ifdef CFI_NEW_KERNEL
#define pgd_offset_raw pgd_offset_pgd
#define mmap_sem mmap_lock
//#endif

// #define CONFIG_DEBUG_PRINTK
#define DEV_FILENAME "XIYUE" //当前驱动DEV文件名

#include "fixFork.h"

#include <linux/kernel.h>
#include <linux/module.h>


#include <asm-generic/mman-common.h>
#include <asm/io.h>
#include <asm/mmu_context.h>
#include <asm/page.h>
#include <asm/pgalloc.h>
#include <asm/pgtable.h>
#include <asm/tlbflush.h>
#include <asm/tlb.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/dax.h>
#include <linux/dcache.h>
#include <linux/debugfs.h>
#include <linux/delayacct.h>
#include <linux/delay.h>
// #include <linux/dma-debug.h>
#include <linux/elf.h>
#include <linux/err.h>
#include <linux/errno.h>
#include <linux/export.h>
#include <linux/fs.h>
#include <linux/gfp.h>
#include <linux/gpio.h>
#include <linux/highmem.h>
#include <linux/hugetlb.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/kallsyms.h>
#include <linux/kernel_stat.h>
#include <linux/ksm.h>
#include <linux/kthread.h>
#include <linux/limits.h>
#include <linux/list.h>
#include <linux/memcontrol.h>
#include <linux/memremap.h>
#include <linux/migrate.h>
#include <linux/miscdevice.h>
#include <linux/mman.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/mmu_notifier.h>
#include <linux/moduleparam.h>
#include <linux/oom.h>
#include <linux/pagemap.h>
#include <linux/path.h>
#include <linux/pci.h>
#include <linux/pfn_t.h>
#include <linux/pid.h>
#include <linux/proc_fs.h>
#include <linux/rmap.h>
#include <linux/security.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/swap.h>
#include <linux/sched.h>
#include <linux/swapops.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/userfaultfd_k.h>
#include <linux/version.h>
#include <linux/writeback.h>
#include <trace/events/kmem.h>



#define 转化内存 '\x01'
#define 指定进程 '\x02'
#define 读取内存 '\x03'
#define 写入内存 '\x04'
#define 获取基址 '\x05'
#define 获取cmdline地址 '\x06'
#define 目标地址 映射区->数据体[线程标号].地址
#define 用户指令 映射区->数据体[线程标号].指令
#define 目标pid 映射区->数据体[线程标号].地址
#define 写入内容 映射区->数据体[线程标号].数据
#define 密码 映射区->数据体[线程标号].数据
#define 返回值 映射区->数据体[线程标号].返回信息

#define 成功 '\x11'
#define 失败 '\x10'

#define 判断(x); if(x>0){返回值=成功;}else{返回值=失败; return -1;}


typedef struct
{
	char 指令;
	char 返回信息;
	size_t 大小;
	uint64_t 地址;
	char 数据[512];
}DAT;

struct 桥梁
{
	DAT 数据体[MAX_THREAD];
};



static struct 桥梁 *映射区;

pgd_t *tag_pgd = NULL;


#ifdef CONFIG_DEBUG_PRINTK
#define printk_debug printk
#else
static inline void printk_debug(char *fmt, ...) {}
#endif


#ifndef xlate_dev_mem_ptr
#define xlate_dev_mem_ptr(p) __va(p)
#endif

#ifndef unxlate_dev_mem_ptr
#define unxlate_dev_mem_ptr(phys, addr)
#endif

#endif /* CONFIG_INI */