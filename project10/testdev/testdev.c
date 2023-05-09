#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>

/* 资源resource */
/*
static struct resource vpu_resources[] = {
    [0] = {
        .start = 0,
        .end = 0,
        .flags = IORESOURCE_MEM,
    },
    [1] = {
        .start = 0,
        .end = 0,
        .flags = IORESOURCE_MEM,
    },
};*/

struct vpu_t {
    char *name;
    unsigned int id;
};


static struct vpu_t vpu_core[] ={
        {"vpu0",0},
        {"vpu1",1},
};



static void vpu_release(struct device *dev) {
    printk("vpu release\n");
}

static struct platform_device vpudevice = {
    .name = "vpu",         // 和platform_driver 的name 要匹配
    .id = -1,
//    .num_resources = ARRAY_SIZE(vpu_resources),
//    .resource = vpu_resources,
    .dev = {
        .release = vpu_release,
        .platform_data = (void *)vpu_core,
    },
};

/* 设备模块加载 */
static int vpudevice_init(void){
    return platform_device_register(&vpudevice);
}

/* 设备模块卸载 */
static void vpudevice_exit(void){
    platform_device_unregister(&vpudevice);
}

module_init(vpudevice_init);
module_exit(vpudevice_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lirui");
