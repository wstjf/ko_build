
#include "fixFork.h"
#include "func.h"
#include "ini.h"

#ifndef VM_RESERVED
#define VM_RESERVED (VM_DONTEXPAND | VM_DONTDUMP)
#endif

#define 大小 映射区->数据体[线程标号].大小



static ssize_t 读操作(struct file *file, char __user *buf, size_t 线程标号, loff_t *ppos)
{
	int 结果;
	struct mm_struct *mm;
	struct task_struct *tag_task;
	struct pid *proc_pid_struct;

	switch (用户指令)
	{
	case 指定进程:
		proc_pid_struct = find_get_pid(目标pid);
		判断(proc_pid_struct);
		printk_debug("proc_pid_struct 0x%lx\n", proc_pid_struct);
		tag_task = pid_task(proc_pid_struct, PIDTYPE_PID);
		判断(tag_task);
		printk_debug("tag_task 0x%lx\n", tag_task);

		mm = get_task_mm(tag_task);
		判断(mm);
     

		down_read(&mm->mmap_sem);
		tag_pgd = mm->pgd;
		up_read(&mm->mmap_sem);
		mmput(mm);
		判断(tag_pgd);
		printk_debug("tag_pgd 0x%lx\n", tag_pgd);

		break;

	case 转化内存:
	
		结果 = 虚拟地址转物理地址(写入内容, 目标地址, tag_pgd);
      if(!结果)
		{
			映射区->数据体[线程标号].返回信息=失败;
			return -1;
		}
		映射区->数据体[线程标号].返回信息=成功;
		break;
	case 读取内存:
		结果 = 读取物理内存(写入内容,目标地址, 大小);
		if(!结果)
		{
			映射区->数据体[线程标号].返回信息=失败;
			return -1;
		}
		映射区->数据体[线程标号].返回信息=成功;
		
		break;

		// case 获取基址:
		// 结果=GetModuleBase(&(映射区->数据体[线程标号].地址) , 映射区->数据体[线程标号].数据);
		// 判断(结果);
		// break;

		// case 获取cmdline地址:
		// 结果=GetModuleBase(&(映射区->数据体[线程标号].地址) , 映射区->数据体[线程标号].数据);
		// 判断(结果);
		// break;
	}
	return 0;
}

static ssize_t 写操作(struct file *filp, const char __user *buf, size_t 线程标号, loff_t *ppos)
{
	int 结果;
	结果 = 写入物理内存(映射区->数据体[线程标号].数据, 映射区->数据体[线程标号].地址, 大小);
	判断(结果);
	return 0;
}

static int 打开操作(struct inode *inode, struct file *file)
{
	printk_debug("打开驱动了");
	return 0;
}

static int 关闭操作(struct inode *inode, struct file *filp)
{
	printk_debug("关闭驱动了");
	return 0;
}
static int 映射内存(struct file *file, struct vm_area_struct *vma)
{
	unsigned long page;
	unsigned long start = (unsigned long)vma->vm_start;
	// unsigned long end =  (unsigned long)vma->vm_end;
	unsigned long size = (unsigned long)(vma->vm_end - vma->vm_start);

	// 得到物理地址
	page = virt_to_phys(映射区);
	// 将用户空间的一个vma虚拟内存区映射到以page开始的一段连续物理页面上
	if (remap_pfn_range(vma, start, page >> PAGE_SHIFT, size,
						PAGE_SHARED)) // 第三个参数是页帧号，由物理地址右移PAGE_SHIFT得到
		return -EAGAIN;
	strcpy(映射区->数据体[0].数据, "曦月魔改驱动 原作者红牛哥 橘子公益电报@orangeXY");
	return 0;
}

static struct file_operations dev_fops = {
	.owner = THIS_MODULE,
	.open = 打开操作,
	.release = 关闭操作,
	.read = 读操作,
	.write = 写操作,
	.mmap = 映射内存,
};

struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEV_FILENAME,
	.fops = &dev_fops,
};

int __init misc_dev_init(void)
{
	int ret;
	printk("[+] 初始化驱动");
	映射区 = (struct 桥梁 *)kmalloc(PAGE_SIZE, GFP_KERNEL);
	SetPageReserved(virt_to_page(映射区));
	ret = misc_register(&misc);
	return ret;
}

void __exit misc_dev_exit(void)
{
	printk("[+] 卸载驱动");
	ClearPageReserved(virt_to_page(映射区));

	kfree(映射区);

	misc_deregister(&misc);
}

module_init(misc_dev_init);
module_exit(misc_dev_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Linux default module");
MODULE_INFO(intree, "Y");
MODULE_INFO(scmversion, "gf9d99a97a122");
MODULE_AUTHOR("Xi Yue designed魔改非原创");
