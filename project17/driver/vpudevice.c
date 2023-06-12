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

static struct vpu_data_t vpudata[] = {
    {"VPU0",0x0, 0, 0x0, 0x400, {}},
    {"VPU1",0x0, 1, 0x0, 0x400, {}},
    {"VPU2",0x0, 2, 0x0, 0x400, {}},
    {"VPU3",0x0, 3, 0x0, 0x400, {}},
};

static void vpu_release(struct device *dev) {
    struct platform_device* pdev = (struct platform_device*)container_of(dev, struct platform_device, dev);
    printk(KERN_INFO "[%s:%d] %s %d\n", __FUNCTION__, __LINE__, pdev->name, pdev->id);
}

static struct platform_device vpupdev[] = {
    [0]={
        .name = "vpudev",
        .id   = 0,
        .dev  = {
               .release = vpu_release,
               .platform_data = (void *)&vpudata[0],
        },
    },
    [1]={
        .name = "vpudev",
        .id   = 1,
        .dev  = {
               .release = vpu_release,
               .platform_data = (void *)&vpudata[1],
        },
    },
    [2]={
        .name = "vpudev",
        .id   = 2,
        .dev  = {
               .release = vpu_release,
               .platform_data = (void *)&vpudata[2],
        },
    },
    [3]={
        .name = "vpudev",
        .id   = 3,
        .dev  = {
               .release = vpu_release,
               .platform_data = (void *)&vpudata[3],
        },
    },
};


int vpu_init(void) {
    int i = 0;
    for (i = 0; i < VPU_DEVICE_NUM; ++i) {
        vpudata[i].vbuffer = kmalloc(vpudata[i].vbufsize, GFP_KERNEL);
        memset(vpudata[i].vbuffer, 0x00, vpudata[i].vbufsize);
        vpudata[i].vid = vpupdev[i].id;
        platform_device_register(&vpupdev[i]);
    }
    printk(KERN_INFO "[%s:%d] %d devices init\n", __FUNCTION__, __LINE__, VPU_DEVICE_NUM);
    return 0;
}
EXPORT_SYMBOL_GPL(vpu_init);

void vpu_exit(void) {
    int i = 0;
    for (i = 0; i < VPU_DEVICE_NUM; ++i) {
        platform_device_unregister(&vpupdev[i]);
        kfree(vpudata[i].vbuffer);
    }
    printk(KERN_INFO "[%s:%d] %d devices exit\n", __FUNCTION__, __LINE__, VPU_DEVICE_NUM);
}
EXPORT_SYMBOL_GPL(vpu_exit);
