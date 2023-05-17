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

#include "../vpudevice.h"

static struct vpu_driver_t nvdriver;


static int vpudrv_open(struct inode *_inode, struct file *fp)
{
    dev_t devNum;
    int index = 0;
    devNum = _inode->i_rdev;
    index = MINOR(devNum);
    fp->private_data = &(nvdriver.vpudev->vdev[index]);
    printk("vpu driver open <%s %d> <%d:%d> success\n", nvdriver.vpudev->vdev[index].name, nvdriver.vpudev->vdev[index].id, MAJOR(devNum), MINOR(devNum));
    return 0;
}

static int vpudrv_release(struct inode *_inode, struct file *fp) {
    struct vdev_t* vdevp = (struct vdev_t*)fp->private_data;
    printk("release %s %x\n",vdevp->name, vdevp->id);
    return 0;
}

static ssize_t vpudrv_read(struct file *fp, char __user *buf, size_t size, loff_t *off)
{
    int ret = 0, type = 0;
    struct vdev_t* vdevp = (struct vdev_t*)fp->private_data;
    copy_from_user(&type, buf, sizeof(unsigned int));
    if(size > vdevp->vbufsize)  {
        printk("size(%d) is out of range(%d)\n", size, vdevp->vbufsize);
        //return -1;
        size = vdevp->vbufsize;
    }
    if (type == 1) {//
        size = sizeof(unsigned int);
        ret = copy_to_user(buf, &vdevp->id, size);
    } else {
        ret = copy_to_user(buf, vdevp->vbuffer, size);        
    }

    if(-1 == ret) {
        printk("read failed\n");
        return -1;        
    }

    printk("read %s %d\n", vdevp->vbuffer, size);
    return size;
}

static ssize_t vpudrv_write(struct file *fp, const char __user *buf, size_t size, loff_t *off)
{
    struct vdev_t* vdevp = (struct vdev_t*)fp->private_data;
    int ret = 0;
    if(size > vdevp->vbufsize)
    {
        printk("size(%d) is out of range(%d)\n", size, vdevp->vbufsize);
        //return -1;
        size = vdevp->vbufsize;
    }

    ret = copy_from_user(vdevp->vbuffer, buf, size);    
    if(-1 == ret)
    {
        printk("write failed\n");
        return -1;        
    }

    printk("write %s %d\n", vdevp->vbuffer, size);
    return size;
}

static int vpudrv_mmap(struct file* fp, struct vm_area_struct* vma)
{
    struct vdev_t* vdevp = (struct vdev_t*)fp->private_data;
    vma->vm_flags |= VM_IO;
    if(remap_pfn_range(vma, 
                       vma->vm_start, 
                       virt_to_phys(vdevp->vbuffer) >> PAGE_SHIFT, 
                       vma->vm_end - vma->vm_start, 
                       vma->vm_page_prot))
    {
        return -EAGAIN;
    }
    printk("vpudrv_mmap\n");
    return 0;
}

static long vpudrv_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int ret = 0;
    printk("vpudrv_ioctl cmd:%d\n", cmd);
    switch (cmd) {
        case VDRV_IOCTL_VERSION://VERSION
            ret = copy_to_user((void __user *)arg, nvdriver.version, 32);
            break;
        case VDRV_IOCTL_RESET://RESET
            break;
        case VDRV_IOCTL_WAIT://WAIT
            break;
        case VDRV_IOCTL_ALLOC://ALLOC
            break;
        case VDRV_IOCTL_FREE://FREE
            break;
        default:
            printk("error vpudrv_ioctl cmd\n");
            break;
    }

    return 0;
}

static struct file_operations fops={
    .owner = THIS_MODULE,
    .open = vpudrv_open,
    .release = vpudrv_release,
    .read = vpudrv_read,
    .write = vpudrv_write,
    .unlocked_ioctl = vpudrv_ioctl,
    .mmap = vpudrv_mmap,
};


static int vpudrv_probe(struct platform_device *pdev)
{
    int ret = 0, i = 0;
    printk("probe\n");
    /*1 dev number*/
    nvdriver.vpudev = (struct vpu_device_t *)pdev->dev.platform_data;
    nvdriver.vdev   = 0;

    ret = alloc_chrdev_region(&nvdriver.vdev, 0, nvdriver.vpudev->ndev, "VPU");
    if (ret != 0) {
        goto ERROR0;
    }
    
    /*2 init cdev */
    nvdriver.vpucdev = cdev_alloc();
    cdev_init(nvdriver.vpucdev,&fops);
    nvdriver.vpucdev->owner = THIS_MODULE;
    
    /*3 add cdev */
    cdev_add(nvdriver.vpucdev, nvdriver.vdev, nvdriver.vpudev->ndev);
    
    /*4 create dev file */
    nvdriver.vpuclass = class_create(THIS_MODULE, "vpudriver");
    for(i = 0;i < nvdriver.vpudev->ndev; ++i) {
	   device_create(nvdriver.vpuclass, NULL, MKDEV(MAJOR(nvdriver.vdev), i), NULL, nvdriver.vpudev->vdev[i].name);
    }
    return 0;

ERROR4:
    /* 释放创建成功的设备节点 */
    for (i--; i >= 0; i--) {
        device_destroy(nvdriver.vpuclass, MKDEV(MAJOR(nvdriver.vdev), i));
    }

    /* 释放设备节点对象 */
    class_destroy(nvdriver.vpuclass);
ERROR3:

ERROR2:
    /* 释放字符设备结构体空间 */
    cdev_del(nvdriver.vpucdev);
ERROR1:
    /* 释放设备号 */
    unregister_chrdev_region(nvdriver.vdev, nvdriver.vpudev->ndev);
ERROR0:
    return ret;
}

static int vpudrv_remove(struct platform_device *pdev)
{
    int i = nvdriver.vpudev->ndev;
    printk("remove\n");
    for (i--; i >= 0; i--)
        device_destroy(nvdriver.vpuclass, MKDEV(MAJOR(nvdriver.vdev), i));
    class_destroy(nvdriver.vpuclass);

    cdev_del(nvdriver.vpucdev);

    unregister_chrdev_region(nvdriver.vdev, nvdriver.vpudev->ndev);
    return 0;
}

static struct platform_driver vpudrv = {
    .probe = vpudrv_probe,
    .remove = vpudrv_remove,
    .driver = {
        .name = "vpudev",
    },
};


static int vpudrv_init(void)
{
    sprintf(nvdriver.version, "1.0.0.1");
    platform_driver_register(&vpudrv);
    return 0;
}

static void vpudrv_exit(void)
{
    platform_driver_unregister(&vpudrv);
}


module_init(vpudrv_init);
module_exit(vpudrv_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("lirui");
MODULE_DESCRIPTION("vpu platform driver");
