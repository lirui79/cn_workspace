#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#include <linux/platform_device.h>

struct led_t {
    char *name;
    unsigned int gpio;
} ;

static struct led_t *led_p;
static int tiny4412_led_open(struct inode *_inode, struct file *fp)
{
    int i;
    for(i=0;i<4;i++){
        led_p[i].gpio = i + 0x10;
    }
    
    printk("open\n");
    return 0;
}

static int tiny4412_led_release(struct inode *_inode, struct file *fp)
{
    int i;
    for(i=0;i<4;i++){
        led_p[i].gpio = 0;
    }
    printk("release\n");
    return 0;
}

static char value = 0xff;

static ssize_t tiny4412_led_read(struct file *fp, char __user *buf, size_t size, loff_t *off)
{
    copy_to_user(buf, &value, sizeof(char));
    printk("read %x\n", value);
    return sizeof(char);
}

static ssize_t tiny4412_led_write(struct file *fp, const char __user *buf, size_t size, loff_t *off)
{
    copy_from_user(&value, buf, sizeof(char));

    if(value &  (0x1<<0))    led_p[0].gpio = 0x20;
    else    led_p[0].gpio = 1;

    if(value &  (0x1<<1))    led_p[1].gpio = 0x30;
    else    led_p[1].gpio = 2;

    if(value &  (0x1<<2))    led_p[2].gpio = 0x40;
    else    led_p[2].gpio = 3;

    if(value &  (0x1<<3))    led_p[3].gpio = 0x50;
    else    led_p[3].gpio = 4;
    printk("write %x\n", value);
    
    return sizeof(char);
}

static struct file_operations fops={
    .owner = THIS_MODULE,
    .open = tiny4412_led_open,
    .release = tiny4412_led_release,
    .read = tiny4412_led_read,
    .write = tiny4412_led_write,
};

#define TINY4412_LED_NAME "tiny4412_led"
static int major = 0;
static struct cdev *led_cdev;
static struct class *led_class;

static int led_probe(struct platform_device *pdev)
{
    printk("probe\n");
    /*1 dev number*/
    led_p = (struct led_t *)pdev->dev.platform_data;
    
    dev_t devnum = 0;
    alloc_chrdev_region(&devnum, 0, 1, TINY4412_LED_NAME);
    major = MAJOR(devnum);
    
    /*2 init cdev */
    led_cdev = cdev_alloc();
    cdev_init(led_cdev,&fops);
    led_cdev->owner = THIS_MODULE;
    
    /*3 add cdev */
    cdev_add(led_cdev, devnum,1);
    
    /*4 create dev file */
    led_class=class_create(THIS_MODULE, TINY4412_LED_NAME);
    device_create(led_class, NULL, devnum, NULL, TINY4412_LED_NAME); //  /dev/tiny4412_led
    return 0;
}

static int led_remove(struct platform_device *pdev)
{
    printk("remove\n");
    device_destroy(led_class, MKDEV(major,0));
    class_destroy(led_class);

    cdev_del(led_cdev);

    unregister_chrdev_region(MKDEV(major,0), 1);
    return 0;
}

static struct platform_driver leddrv = {
    .probe = led_probe,
    .remove = led_remove,
    .driver = {
        .name = "tiny4412_led",
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