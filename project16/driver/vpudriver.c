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
    struct cdev        vcdev;
    dev_t              vdev;
    int                ndev;
    char               version[16];
    struct class      *vclass;
    struct vpu_data_t *vdata;
};

static struct vpu_driver_t vpudriver = {{}, 0, 0x04, "1.0.0.3", NULL, NULL};

static int vpudrv_open(struct inode *_inode, struct file *fp)
{
    struct vpu_driver_t *vdrv =  (struct vpu_driver_t*)container_of(_inode->i_cdev, struct vpu_driver_t, vcdev);
    struct vpu_data_t *vdata = &(vdrv->vdata[MINOR(_inode->i_rdev)]);
    fp->private_data = vdata;
    printk("vpu driver open <%s %d> <%d:%d> success\n", vdata->vname, vdata->vid, MAJOR(_inode->i_rdev), vpudriver.vdev);
    return 0;
}

static int vpudrv_release(struct inode *_inode, struct file *fp) {
    struct vpu_data_t *vdata = (struct vpu_data_t*)fp->private_data;
    printk("release %s %d\n",vdata->vname, vdata->vid);
    return 0;
}

static ssize_t vpudrv_read(struct file *fp, char __user *buf, size_t size, loff_t *off)
{
    int ret = 0;
    struct vpu_data_t *vdata = (struct vpu_data_t*)fp->private_data;
    if(size > vdata->vbufsize)  {
        printk("size(%d) is out of range(%d)\n", size, vdata->vbufsize);
        size = vdata->vbufsize;
    }
    if (size == sizeof(unsigned int)) {//
        ret = copy_to_user(buf, &vdata->vid, size);
    } else {
        ret = copy_to_user(buf, vdata->vbuffer, size);        
    }

    if(-1 == ret) {
        printk("read failed\n");
        return -1;        
    }

    printk("read %p %d\n", (char*)vdata->vbuffer, size);
    return size;
}

static ssize_t vpudrv_write(struct file *fp, const char __user *buf, size_t size, loff_t *off)
{
    int ret = 0;
    struct vpu_data_t *vdata = (struct vpu_data_t*)fp->private_data;
    if(size > vdata->vbufsize) {
        printk("size(%d) is out of range(%d)\n", size, vdata->vbufsize);
        size = vdata->vbufsize;
    }

    ret = copy_from_user(vdata->vbuffer, buf, size);    
    if(-1 == ret) {
        printk("write failed\n");
        return -1;        
    }

    printk("write %p %d\n", (char*)vdata->vbuffer, size);
    return size;
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
    printk("vpudrv_mmap\n");
    return 0;
}

static long vpudrv_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    int ret = 0;
    printk("vpudrv_ioctl cmd:%d\n", cmd);
    switch (cmd) {
        case VDRV_IOCTL_VERSION://VERSION
            ret = copy_to_user((void __user *)arg, vpudriver.version, 16);
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


static int vpudrv_probe(struct platform_device *pdev) {
    int ret = 0, i = 0;
    struct device *devices = NULL;
    printk("probe\n");
    /*1 dev number*/
    struct vpu_data_t *vdata = (struct vpu_data_t *)pdev->dev.platform_data;
    if (NULL == vpudriver.vdata) {
        vpudriver.vdata = vdata;
    }
    devices = device_create(vpudriver.vclass, NULL, MKDEV(MAJOR(vpudriver.vdev), vdata->vid), NULL, vdata->vname);
    if (devices == NULL) {
        
    }
    return 0;
}

static int vpudrv_remove(struct platform_device *pdev) {
    struct vpu_data_t *vdata = (struct vpu_data_t *)pdev->dev.platform_data;
    printk("remove\n");
    device_destroy(vpudriver.vclass, MKDEV(MAJOR(vpudriver.vdev), vdata->vid));
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
        return -1;
    }
    /*2 init cdev */    //vdata->vcdev = cdev_alloc();
    cdev_init(&(vpudriver.vcdev),&fops);
    vpudriver.vcdev.owner = THIS_MODULE;
    
    /*3 add cdev */
    ret = cdev_add(&(vpudriver.vcdev), vpudriver.vdev, vpudriver.ndev);
    if (ret != 0) {
        return -2;
    }
    /*4 create dev file */
    vpudriver.vclass = class_create(THIS_MODULE, "vpudriver");
    platform_driver_register(&vpudrv);
    printk("vpudrv:vpudrv_init\n");
    return 0;
}

static void vpudrv_exit(void) {
    platform_driver_unregister(&vpudrv);
    class_destroy(vpudriver.vclass);
    cdev_del(&(vpudriver.vcdev));
    unregister_chrdev_region(vpudriver.vdev, vpudriver.ndev);
    vpu_exit();
    printk("vpudrv:vpudrv_exit\n");
}


module_init(vpudrv_init);
module_exit(vpudrv_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("lirui");
MODULE_DESCRIPTION("vpu platform driver");
