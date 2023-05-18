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

static struct vpu_device_t nvdevice = {
        .ndev = 4,
        .vdev = {
            {"VPU0",0, 0x0, 0x400},
            {"VPU1",1, 0x0, 0x400},
            {"VPU2",2, 0x0, 0x400},
            {"VPU3",3, 0x0, 0x400},
        },
};

static void vpu_release(struct device *dev)
{
    printk("vpudev:vpu_release\n");
}

static struct platform_device vpudev = {
    .name = "vpudev",
    .dev = {
        .release = vpu_release,
        .platform_data = (void *)&nvdevice,
    },
};


static int vpu_init(void)
{
    int i = 0;
    for (i = 0; i < nvdevice.ndev; ++i) {
        nvdevice.vdev[i].vbuffer = kmalloc(nvdevice.vdev[i].vbufsize, GFP_KERNEL);
        memset(nvdevice.vdev[i].vbuffer, 0x00 ,nvdevice.vdev[i].vbufsize);
    }
    platform_device_register(&vpudev);
    printk("vpudev:vpu_init\n");
    return 0;
}

static void vpu_exit(void)
{
    int i = 0;
    for (i = 0; i < nvdevice.ndev; ++i) {
        kfree(nvdevice.vdev[i].vbuffer);
    }
    platform_device_unregister(&vpudev);
    printk("vpudev:vpu_exit\n");
}


module_init(vpu_init);
module_exit(vpu_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("lirui");
MODULE_DESCRIPTION("vpu platform device");