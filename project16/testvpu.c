
/* 应用代码 */
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <sys/ioctl.h>

#include "vpucmd.h"

#define MM_SIZE 1024

int main(void)
{
    int fd = -1, ret = -1, i = 0;
    char devName[32] = {0};
    char buffer[MM_SIZE] = {0};
    char* buf_addr = NULL;
    char version[32] = {0};
    for (i = 0; i < 4; ++i) {
		sprintf(devName,"/dev/VPU%d", i);
		fd = open(devName,O_RDWR);
		unsigned int wvalue = 0x12323 + i, rvalue = 0;
		buf_addr = mmap(NULL, MM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		ret = read(fd, &rvalue, sizeof(unsigned int));
		printf("read %d %x\n", ret, rvalue);
		ret = write(fd, &wvalue, sizeof(unsigned int));
		printf("write %d %x\n", ret, wvalue);
		ret = ioctl(fd, VDRV_IOCTL_VERSION, (void*)version);
		printf("version:%s\n", version);
		bzero(buffer, MM_SIZE);
		write(fd, buffer, MM_SIZE);
		write(fd, "write_device_mmap", strlen("write_device_mmap"));
		printf("write buffer: %s\n", buf_addr);

		munmap(buf_addr, MM_SIZE);
		close(fd);
    }
    
    return 0;
}
