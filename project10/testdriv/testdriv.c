/* 内核驱动 */
#include <linux/module.h>   
#include <linux/init.h>  
#include <linux/fs.h>   
#include <linux/device.h>   
#include <linux/slab.h>  
#include <linux/cdev.h>  
#include <linux/err.h>
#include <linux/mm.h>  
#include <linux/mm_types.h>  
#include <asm/uaccess.h>  
#include <linux/io.h>  
#include <linux/platform_device.h>
#include <linux/kern_levels.h>
#include <linux/ioport.h>      
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/uaccess.h>


#define DEVICE_MM_SIZE 4096

struct test_device
{
    dev_t n_dev;            
    struct cdev cdev;     
    struct class *dev_class;
    struct device *dev;  
    u8*  share_buffer;
    u32  buffer_size;  
};

static int open(struct inode* inode, struct file* filp)
{   
    printk("device open, filp = %p, f_owner.pid = %p\n", filp, filp->f_owner.pid);
    filp->private_data = (void*)container_of(inode->i_cdev, struct test_device, cdev);
    return 0;
}

static ssize_t read(struct file* filp, char __user * buf, size_t size, loff_t* ppos)
{
    int ret = 0;
    struct test_device* test_device = (struct test_device*)filp->private_data;
    if(size > test_device->buffer_size)
    {
        printk("size(%d) is out of range(%d)\n", size, test_device->buffer_size);
        return -1;
    }

    ret = copy_to_user(buf, test_device->share_buffer, size);
    if(-1 == ret)
    {
        printk("read failed\n");
        return -1;        
    }

    printk("read %s %d\n", test_device->share_buffer, size);
    return size;
}

static ssize_t write(struct file* filp, const char __user * buf, size_t size, loff_t* ppos)
{
    int ret = 0;
    struct test_device* test_device = (struct test_device*)filp->private_data;

    if(size > test_device->buffer_size)
    {
        printk("size(%d) is out of range(%d)\n", size, test_device->buffer_size);
        return -1;
    }

    ret = copy_from_user(test_device->share_buffer, buf, size);    
    if(-1 == ret)
    {
        printk("write failed\n");
        return -1;        
    }

    printk("write %s %d\n", test_device->share_buffer, size);
    return size;
}

static int mmap(struct file* filp, struct vm_area_struct* vma)
{
    struct test_device* test_device = (struct test_device*)filp->private_data;
    vma->vm_flags |= VM_IO;
    if(remap_pfn_range(vma, 
                       vma->vm_start, 
                       virt_to_phys(test_device->share_buffer) >> PAGE_SHIFT, 
                       vma->vm_end - vma->vm_start, 
                       vma->vm_page_prot))
    {
        return -EAGAIN;
    }
    printk("mmap\n");
    return 0;
}

static struct file_operations test_fops = 
{
    .owner = THIS_MODULE, 
    .open = open,
    .read = read,
    .write = write,
    .mmap = mmap,
};

static int vpu_probe(struct platform_device *pdev)
{
    int ret = 0;
    struct test_device* test_device = NULL;

    /* 1. 为设备数据分配空间 */
    test_device = devm_kzalloc(&pdev->dev, sizeof(struct test_device), GFP_KERNEL);
    if(!test_device)
    {
        printk("can't create test_device\n");
        goto ALLOC_FAIL;
    }
    platform_set_drvdata(pdev, test_device);

    /* 2.1 分配设备号 */
    int base_minor = 0;
    int n_num = 1;
    ret = alloc_chrdev_region(&test_device->n_dev, base_minor, n_num, "test_device");
    if(0 != ret)
    {
        printk(KERN_INFO"can't alloc a chrdev number\n");
        goto REGION_FAIL;
    }
    printk("major = %d, minor = %d\n", MAJOR(test_device->n_dev), MINOR(test_device->n_dev));

    /* 2.2 初始化字符设备 */
    int dev_count = 1;
    cdev_init(&test_device->cdev, &test_fops);
    test_device->cdev.owner = THIS_MODULE;
    ret = cdev_add(&test_device->cdev, test_device->n_dev, dev_count);
    if(0 != ret)
    {
        printk(KERN_INFO"add a cdev error\n");
        goto CDEV_FAILE;
    }  

    /* 2.3 创建设备，发出uevent事件 ,在/sys/class/目录下创建设备类别目录dev_class */
    test_device->dev_class = class_create(THIS_MODULE, "vpu_class");
    if(IS_ERR(test_device->dev_class)) 
    {
        printk(KERN_INFO"create a class error\n");
        goto CDEV_FAILE;
    }       

    /* 2.4 在/dev/目录和/sys/class/gpio_class目录下分别创建设备文件gpio_dev */
    test_device->dev = device_create(test_device->dev_class, NULL, test_device->n_dev, NULL, "vpu");
    if(IS_ERR(test_device->dev)) 
    {
        printk(KERN_INFO"create a device error\n");
        goto CLASS_FAILE;
    }

    test_device->buffer_size = DEVICE_MM_SIZE;
#if 1    
    /*
        请注意，要进行mmap映射的内存地址必须是4k对齐，不然应用层使用mmap映射后的地址可能是不对的。
        笔者理解是：因为remap_pfn_range调用会对地址进行检查，将地址映射到4k对齐的虚拟地址。
        举个例子，假如该内存地址是不对齐的，比如为0x0000fb10，那么有可能会导致地址映射到0x0000fb00，这样导致应用层映射到的地址是从0x0000fb00开始
    */
    test_device->share_buffer = kmalloc(test_device->buffer_size, GFP_KERNEL);
#else
    /*
        使用devm_kzalloc会导致出来的地址并不是4k对齐，从而导致应用层会发生映射错误
        笔者一开始使用的是devm_kzalloc分配，分配出来的地址为0xc3b96010，从而导致应用层读取错误，需要加上16的便宜才能访问到正确的地址
        修改为kmalloc，分配出来的地址是0xb6f9a000，该地址是4k对齐，所以应用层映射后的地址是正确的。
    */
    test_device->share_buffer = devm_kzalloc(&pdev->dev, test_device->buffer_size, GFP_KERNEL);
#endif
    if(!test_device->share_buffer)
    {
        printk("can't create share buffer\n");
        goto ALLOC_BUFFER_FAIL;
    }

    printk("vpu_probe %p %d\n", test_device->share_buffer, test_device->buffer_size);
    return 0;
ALLOC_BUFFER_FAIL:
    device_destroy(test_device->dev_class, test_device->n_dev);
CLASS_FAILE:
    class_destroy(test_device->dev_class);
CDEV_FAILE:    
    unregister_chrdev_region(test_device->n_dev, n_num); 
REGION_FAIL:    
    devm_kfree(&pdev->dev, test_device); 
ALLOC_FAIL:
    return -1;
} 

static int vpu_remove(struct platform_device *pdev)
{    
    struct test_device* test_device = (struct test_device*)platform_get_drvdata(pdev);
    int n_num = 1;
#if 1  
    kfree(test_device->share_buffer);
#else
    devm_kfree(&pdev->dev, test_device->share_buffer);
#endif    
    device_destroy(test_device->dev_class, test_device->n_dev);
    class_destroy(test_device->dev_class);
    cdev_del(&test_device->cdev);
    unregister_chrdev_region(test_device->n_dev, n_num); 
    devm_kfree(&pdev->dev, test_device);
    printk("vpu_remove\n");
    return 0;
}  


static struct platform_driver vpu_driver = {
    .probe  = vpu_probe,
    .remove = vpu_remove,
    .driver = {
        .name   = "vpu",
    },
};


static int __init testdev_init(void)
{
	int res;
	printk("[VPUDRV] begin vpu_init\n");
    res = platform_driver_register(&vpu_driver);
	printk("[VPUDRV] end vpu_init result=0x%x\n", res);
	return res;
}

static void __exit testdev_exit(void)
{
	platform_driver_unregister(&vpu_driver);
    printk("[VPUDRV] vpu_exit\n");
	return;
}

MODULE_AUTHOR("A customer using C&M testdev, Inc.");
MODULE_DESCRIPTION("vpu linux driver");
MODULE_LICENSE("GPL");

module_init(testdev_init);
module_exit(testdev_exit);
