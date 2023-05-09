#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>

#include "../testvpu.h"

static struct led_t tiny4412leds[] ={
        {"LED1",0},
        {"LED2",1},
        {"LED3",2},
        {"LED4",3},
};

static void led_release(struct device *dev)
{
}

static struct platform_device leddev = {
    .name = "tinyled",
    .dev = {
        .release = led_release,
        .platform_data = (void *)tiny4412leds,
    },
};


static int led_init(void)
{
    platform_device_register(&leddev);
    return 0;
}

static void led_exit(void)
{
    platform_device_unregister(&leddev);
}


module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");