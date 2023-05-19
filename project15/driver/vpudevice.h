
#ifndef  __VPU_DEVICE_T_H__
#define  __VPU_DEVICE_T_H__



struct vpu_data_t{
    char         *vname;
    unsigned int  vid;
    dev_t         vdev;
    void         *vbuffer;
    unsigned int  vbufsize;
};

int   vpu_init(void);
void  vpu_exit(void);

#endif  //__VPU_DEVICE_T_H__