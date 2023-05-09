
#include <linux/device.h> 
#include <linux/ioctl.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/mm.h>
#include<linux/sched.h>
#include<linux/cdev.h>
#include<asm/io.h>
#include<asm/switch_to.h>
#include<asm/uaccess.h>
#include<linux/kernel.h>

#define DRIVERNAME "mycdev"
#define DEV_MINOR 0

#define MYCDEV_SIZE 1024
static dev_t mycdev_devno = 0;
static struct class * mycdev_class;

struct mycdev_dev 
{
	struct cdev cdev;	
}mycdevdev;

/*-----------------------------------------------------------------------------------*/

static int mycdev_open(struct inode *inode, struct file *filp) {
  printk("mycdev_open\n");
	return 0;
}

static ssize_t mycdev_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos)
{
  unsigned long p = *ppos;
  unsigned int count = size;
  char kernel_buf[MYCDEV_SIZE] = "This is mycdev!";
  int i;

  if(p >= MYCDEV_SIZE)
    return -1;
  if(count > MYCDEV_SIZE)
    count = MYCDEV_SIZE - p;
  if(copy_to_user(buf,kernel_buf,count) != 0){
      printk("read error!\n");

      return -1;
    }

  printk("reader:%d bytes was read...\n",count);
  return count;
}

static ssize_t mycdev_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos) {
  printk("mycdev_write\n");
	return 0;
}

static int mycdev_release(struct inode *inode, struct file *filp) {  
  printk("mycdev_release\n");
	return 0;
}

static long mycdev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = 0;	
 
  printk("mycdev_ioctl cmd:%d\n", cmd);
	switch (cmd) 
	{
		case 0://CPLD_VERSION:
			break;
		case 1://CPLD_RESET:
			break;
		default:
			printk("Error mycdev cmd\n");
			break;
	}
	
	return 0;
}

/*-----------------------------------------------------------------------------------*/

static struct file_operations mycdev_fops = {
	.owner = THIS_MODULE,
	.open = mycdev_open,
	.read = mycdev_read,
	.write = mycdev_write,
	.compat_ioctl = mycdev_ioctl,
	.release = mycdev_release,
};

static int mycdev_init(void)
{
	int ret = 0;

	/*动态申请设备号*/
	ret = alloc_chrdev_region(&mycdev_devno, DEV_MINOR, 1, DRIVERNAME);
	if (ret < 0)
	{
		printk("mycdev register char dev failed\n");
		goto out;
	}
	/*初始化cdev结构，注冊cdev*/
	cdev_init(&(mycdevdev.cdev), &mycdev_fops);
	mycdevdev.cdev.owner = THIS_MODULE;
	ret = cdev_add(&(mycdevdev.cdev), mycdev_devno, 1);
	if (ret) 
	{
		printk("Error adding mycdev device\n");
		goto add_err;
	}
	
	/*创建设备节点/sys/class/mycdev和/dev/mycdev*/
	mycdev_class = class_create(THIS_MODULE, DRIVERNAME);
	if (IS_ERR(mycdev_class)) 
	{
		ret = PTR_ERR(mycdev_class);
		goto class_err;
	}
	device_create(mycdev_class, NULL, mycdev_devno, NULL, DRIVERNAME); 
	
	printk("mycdev init\n");
	return 0;

class_err:
	cdev_del(&(mycdevdev.cdev));
add_err:	
	unregister_chrdev_region(mycdev_devno, 1);	
out:
	return ret;
}

static void mycdev_exit(void)
{
	device_destroy(mycdev_class, mycdev_devno);
	class_destroy(mycdev_class);
	cdev_del(&(mycdevdev.cdev));
	unregister_chrdev_region(mycdev_devno, 1);
	printk("module mycdev exit\n");
}

module_init(mycdev_init);
module_exit(mycdev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liuwei");
MODULE_DESCRIPTION("char driver");