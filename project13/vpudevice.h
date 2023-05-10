
#ifndef  __VPU_DEVICE_T_H__
#define  __VPU_DEVICE_T_H__

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
struct vdev_t{
    char *name;
    unsigned int id;
    void *vbuffer;
    unsigned int vbufsize;
} ;

struct vpu_device_t {
    unsigned int  ndev;
    struct vdev_t vdev[4];
};

struct vpu_driver_t {
    dev_t                 vdev;
    struct vpu_device_t  *vpudev;
    struct cdev          *vpucdev;
    struct class         *vpuclass;
};


#endif  //__VPU_DEVICE_T_H__