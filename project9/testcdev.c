
/* 应用代码 */
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>

#define MM_SIZE 4096

int main(int argc, char* argv[])
{
    int dev_fd = 0;
    char* buf_addr = NULL;
    char buffer[MM_SIZE] = {0};

    dev_fd = open("/dev/test_dev", O_RDWR);//必须设置为O_RDWR，不然无法使用MAP_SHARED映射标志
    if(-1 == dev_fd)
    {
        printf("open device error\n");
        return -1;
    }

    buf_addr = mmap(NULL, MM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, dev_fd, 0) + 16;
    
    strcpy(buf_addr, "read_device_mmap");
    bzero(buffer, MM_SIZE);
    int ret = read(dev_fd, buffer, MM_SIZE);
    printf("read buffer: %s\n", buffer);

    
    bzero(buffer, MM_SIZE);
    write(dev_fd, buffer, MM_SIZE);
    write(dev_fd, "write_device_mmap", strlen("write_device_mmap"));
    printf("write buffer: %s\n", buf_addr);

    munmap(buf_addr, MM_SIZE);
	return 0;
}
