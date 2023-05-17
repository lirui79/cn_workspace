
#ifndef  __VPU_DEVICE_T_H__
#define  __VPU_DEVICE_T_H__



#define VDRV_IOCTL_MAGIC       'V'
#define VDRV_IOCTL_VERSION     _IO(VDRV_IOCTL_MAGIC, 0)
#define VDRV_IOCTL_RESET       _IO(VDRV_IOCTL_MAGIC, 1)
#define VDRV_IOCTL_WAIT        _IO(VDRV_IOCTL_MAGIC, 2)
#define VDRV_IOCTL_ALLOC       _IO(VDRV_IOCTL_MAGIC, 3)
#define VDRV_IOCTL_FREE        _IO(VDRV_IOCTL_MAGIC, 4)


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
    char                  version[32];
};



#endif  //__VPU_DEVICE_T_H__