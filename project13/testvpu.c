
/* 应用代码 */
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>

#define MM_SIZE 1024

int main(void)
{
    int fd, ret, i ;
    char devName[32] = {0};
    char buffer[MM_SIZE] = {0};
    char* buf_addr = NULL;
    for (i = 0; i < 4; ++i) {
				sprintf(devName,"/dev/VPU%d", i);
				fd = open(devName,O_RDWR);
				unsigned int wvalue = 0x12323 + i, rvalue = 0;
				buf_addr = mmap(NULL, MM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
				ret = read(fd, &rvalue, sizeof(unsigned int));
				printf("read %d %x\n", ret, rvalue);
				ret = write(fd, &wvalue, sizeof(unsigned int));
				printf("write %d %x\n", ret, wvalue);
    
		    bzero(buffer, MM_SIZE);
		    write(fd, buffer, MM_SIZE);
		    write(fd, "write_device_mmap", strlen("write_device_mmap"));
		    printf("write buffer: %s\n", buf_addr);

		    munmap(buf_addr, MM_SIZE);
				close(fd);
    }
    
    return 0;
}
