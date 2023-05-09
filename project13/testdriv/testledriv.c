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

static struct led_t *led_p;
static int tiny4412_led_open(struct inode *_inode, struct file *fp)
{
    dev_t devNum;
    int index = 0;
    devNum = _inode->i_rdev;
    index = MINOR(devNum);
    fp->private_data = &led_p[index];
    printk("led driver open <%s %d> <%d:%d> success\n", led_p[index].name, led_p[index].gpio, MAJOR(devNum), MINOR(devNum));
    return 0;
}

static int tiny4412_led_release(struct inode *_inode, struct file *fp)
{
    struct led_t* led = (struct led_t*)fp->private_data;
    printk("release %s %x\n",led->name, led->gpio);
    return 0;
}

static ssize_t tiny4412_led_read(struct file *fp, char __user *buf, size_t size, loff_t *off)
{
    struct led_t* led = (struct led_t*)fp->private_data;
    copy_to_user(buf, &led->gpio, sizeof(unsigned int));
    printk("read %s %x\n",led->name, led->gpio);
    return sizeof(unsigned int);
}

static ssize_t tiny4412_led_write(struct file *fp, const char __user *buf, size_t size, loff_t *off)
{
    struct led_t* led = (struct led_t*)fp->private_data;
    copy_from_user(&led->gpio, buf, sizeof(unsigned int));
    printk("write %s %x\n",led->name, led->gpio);
    
    return sizeof(unsigned int);
}

static struct file_operations fops={
    .owner = THIS_MODULE,
    .open = tiny4412_led_open,
    .release = tiny4412_led_release,
    .read = tiny4412_led_read,
    .write = tiny4412_led_write,
};

//#define TINY4412_LED_NAME "tiny4412_led"
static dev_t devnum = 0;
static struct cdev *led_cdev;
static struct class *led_class;
static int ledNum = 4;

static int led_probe(struct platform_device *pdev)
{
    int ret = 0, i = 0;
    printk("probe\n");
    /*1 dev number*/
    led_p = (struct led_t *)pdev->dev.platform_data;

    ret = alloc_chrdev_region(&devnum, 0, ledNum, "tiny4412_led");
    if (ret != 0) {
        goto ERROR0;
    }
    
    /*2 init cdev */
    led_cdev = cdev_alloc();
    cdev_init(led_cdev,&fops);
    led_cdev->owner = THIS_MODULE;
    
    /*3 add cdev */
    cdev_add(led_cdev, devnum, ledNum);
    
    /*4 create dev file */
    led_class=class_create(THIS_MODULE, "led");
    for(i = 0;i < ledNum; ++i) {
	device_create(led_class, NULL, MKDEV(MAJOR(devnum), i), NULL, led_p[i].name); //  /dev/tiny4412_led
    }
    return 0;

ERROR4:
    /* 释放创建成功的设备节点 */
    for (i--; i >= 0; i--)
        device_destroy(led_class, MKDEV(MAJOR(devnum), i));

    /* 释放设备节点对象 */
    class_destroy(led_class);
ERROR3:

ERROR2:
    /* 释放字符设备结构体空间 */
    cdev_del(led_cdev);
ERROR1:
    /* 释放设备号 */
    unregister_chrdev_region(devnum, ledNum);
ERROR0:
    return ret;
}

static int led_remove(struct platform_device *pdev)
{
    int i = ledNum;
    printk("remove\n");
    for (i--; i >= 0; i--)
        device_destroy(led_class, MKDEV(MAJOR(devnum), i));
    class_destroy(led_class);

    cdev_del(led_cdev);

    unregister_chrdev_region(devnum, ledNum);
    return 0;
}

static struct platform_driver leddrv = {
    .probe = led_probe,
    .remove = led_remove,
    .driver = {
        .name = "tinyled",
    },
};


static int led_init(void)
{
    platform_driver_register(&leddrv);
    return 0;
}

static void led_exit(void)
{
    platform_driver_unregister(&leddrv);
}


module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");
