
#ifndef  __VPU_DEVICE_T_H__
#define  __VPU_DEVICE_T_H__



struct vpu_data_t{
    char         *vname;
    unsigned int  vid;
    dev_t         vdev;
    void         *vbuffer;
    unsigned int  vbufsize;
};



#endif  //__VPU_DEVICE_T_H__