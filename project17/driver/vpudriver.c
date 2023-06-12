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

#include "vpudevice.h"
#include "../vpucmd.h"

struct vpu_driver_t{
    dev_t              vdev;
    int                ndev;
    char               version[16];
    struct class      *vclass;
};

static struct vpu_driver_t vpudriver = {0, 0x04, "1.0.0.3", NULL};

static int vpudrv_open(struct inode *_inode, struct file *fp)
{
    struct vpu_data_t *vdata=  (struct vpu_data_t*)container_of(_inode->i_cdev, struct vpu_data_t, vcdev);
    fp->private_data = vdata;
    printk(KERN_INFO "[%s:%d] open <%s %d> <%d:%d> success\n", __FUNCTION__, __LINE__, vdata->vname, vdata->vid, MAJOR(_inode->i_rdev), vpudriver.vdev);
    return 0;
}

static int vpudrv_release(struct inode *_inode, struct file *fp) {
    struct vpu_data_t *vdata = (struct vpu_data_t*)fp->private_data;
    printk(KERN_INFO "[%s:%d] release <%s %d> success\n", __FUNCTION__, __LINE__, vdata->vname, vdata->vid);
    return 0;
}

static ssize_t vpudrv_read(struct file *fp, char __user *buf, size_t size, loff_t *off)
{
    int ret = 0;
    size_t rsize = size;
    struct vpu_data_t *vdata = (struct vpu_data_t*)fp->private_data;
    if(rsize > vdata->vbufsize)  {
        printk(KERN_WARNING "[%s:%d] read <%s %d> size(%d > %d)\n", __FUNCTION__, __LINE__, vdata->vname, vdata->vid, rsize, vdata->vbufsize);
        rsize = vdata->vbufsize;
    }
    if (rsize == sizeof(unsigned int)) {//
        ret = copy_to_user(buf, &vdata->vid, rsize);
    } else {
        ret = copy_to_user(buf, vdata->vbuffer, rsize);
    }

    if(-1 == ret) {
        printk(KERN_ERR "[%s:%d] read <%s %d> failed\n", __FUNCTION__, __LINE__, vdata->vname, vdata->vid);
        return -1;        
    }

    printk(KERN_DEBUG "[%s:%d] read <%s %d> <%p %d>\n", __FUNCTION__, __LINE__, vdata->vname, vdata->vid, vdata->vbuffer, rsize);
    return rsize;
}

static ssize_t vpudrv_write(struct file *fp, const char __user *buf, size_t size, loff_t *off)
{
    int ret = 0;
    size_t rsize = size;
    struct vpu_data_t *vdata = (struct vpu_data_t*)fp->private_data;
    if(rsize > vdata->vbufsize) {
        printk(KERN_WARNING "[%s:%d] write <%s %d> size(%d > %d)\n", __FUNCTION__, __LINE__, vdata->vname, vdata->vid, rsize, vdata->vbufsize);
        rsize = vdata->vbufsize;
    }
    ret = copy_from_user(vdata->vbuffer, buf, rsize);
    if(-1 == ret) {
        printk(KERN_ERR "[%s:%d] write <%s %d> failed\n", __FUNCTION__, __LINE__, vdata->vname, vdata->vid);
        return -1;        
    }

    printk(KERN_DEBUG "[%s:%d] write <%s %d> <%p %d>\n", __FUNCTION__, __LINE__, vdata->vname, vdata->vid, vdata->vbuffer, rsize);
    return rsize;
}

static int vpudrv_mmap(struct file* fp, struct vm_area_struct* vma)
{
    struct vpu_data_t *vdata = (struct vpu_data_t*)fp->private_data;
    vma->vm_flags |= VM_IO;
    if(remap_pfn_range(vma, 
                       vma->vm_start, 
                       virt_to_phys(vdata->vbuffer) >> PAGE_SHIFT, 
                       vma->vm_end - vma->vm_start, 
                       vma->vm_page_prot))
    {
        return -EAGAIN;
    }
    printk(KERN_INFO "[%s:%d] mmap <%s %d> success\n", __FUNCTION__, __LINE__, vdata->vname, vdata->vid);
    return 0;
}

static long vpudrv_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
    int ret = 0;
    struct vpu_data_t *vdata = (struct vpu_data_t*)fp->private_data;
    printk(KERN_DEBUG "[%s:%d] ioctl <%s %d> %d success\n", __FUNCTION__, __LINE__, vdata->vname, vdata->vid, cmd);
    switch (cmd) {
        case VDRV_IOCTL_VERSION://VERSION
            ret = copy_to_user((void __user *)arg, vpudriver.version, 15);
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
            printk(KERN_ERR "[%s:%d] ioctl <%s %d> %d failed\n", __FUNCTION__, __LINE__, vdata->vname, vdata->vid, cmd);
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


static int vpudrv_probe(struct platform_device *pdev) {
    int ret = 0;
    struct device *devices = NULL;
    /*1 dev number*/
    struct vpu_data_t *vdata = (struct vpu_data_t *)pdev->dev.platform_data;
    printk(KERN_DEBUG "[%s:%d] probe <%s %d>\n", __FUNCTION__, __LINE__, vdata->vname, vdata->vid);
/*2 init cdev */
    cdev_init(&(vdata->vcdev),&fops);
    vdata->vcdev.owner = THIS_MODULE;
/*3 add cdev */
    vdata->vdev = MKDEV(vpudriver.vdev, pdev->id);
    ret = cdev_add(&(vdata->vcdev),  vdata->vdev,1);
    if (ret != 0) {
       printk(KERN_ERR "[%s:%d] probe <%s %d> %d\n", __FUNCTION__, __LINE__, vdata->vname, pdev->id, ret);
       return -1;
    }

    devices = device_create(vpudriver.vclass, NULL, vdata->vdev, NULL, vdata->vname);
    if (devices == NULL) {
       printk(KERN_ERR "[%s:%d] probe <%s %d>\n", __FUNCTION__, __LINE__, vdata->vname, pdev->id);
       return -2;
    }
    return 0;
}

static int vpudrv_remove(struct platform_device *pdev) {
    struct vpu_data_t *vdata = (struct vpu_data_t *)pdev->dev.platform_data;
    device_destroy(vpudriver.vclass, vdata->vdev);
    cdev_del(&(vdata->vcdev));
    printk(KERN_DEBUG "[%s:%d] remove <%s %d>\n", __FUNCTION__, __LINE__, vdata->vname, pdev->id);
    return 0;
}

static const struct platform_device_id vpupdev_id[] = {
    {.name = "vpudev"},
    {},
};

MODULE_DEVICE_TABLE(platform, vpupdev_id);

static struct platform_driver vpudrv = {
    .probe   = vpudrv_probe,
    .remove  = vpudrv_remove,
    .driver  = {
       .name   = "vpudev",
       .owner   = THIS_MODULE,
    },
    .id_table   = vpupdev_id,
};


static int vpudrv_init(void) {
    int ret = 0;
    vpu_init();
    ret = alloc_chrdev_region(&(vpudriver.vdev), 0, vpudriver.ndev, "VPU");
    if (ret != 0) {
        printk(KERN_ERR "[%s:%d] init <%d %d>\n", __FUNCTION__, __LINE__, vpudriver.vdev, ret);
        return -1;
    }
    vpudriver.vdev = MAJOR(vpudriver.vdev);
    printk(KERN_DEBUG "[%s:%d] init <%d %d>\n", __FUNCTION__, __LINE__, vpudriver.vdev, vpudriver.ndev);
    vpudriver.vclass = class_create(THIS_MODULE, "vpudriver");
    platform_driver_register(&vpudrv);
    return 0;
}

static void vpudrv_exit(void) {
    platform_driver_unregister(&vpudrv);
    class_destroy(vpudriver.vclass);
    unregister_chrdev_region(vpudriver.vdev, vpudriver.ndev);
    vpu_exit();
    printk(KERN_DEBUG "[%s:%d] exit <%d %d>\n", __FUNCTION__, __LINE__, vpudriver.vdev, vpudriver.ndev);
}


module_init(vpudrv_init);
module_exit(vpudrv_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("lirui");
MODULE_DESCRIPTION("vpu platform driver");
