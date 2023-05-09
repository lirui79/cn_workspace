#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>


int main(void)
{
    int fd, ret;
    fd = open("/dev/tiny4412_led",O_RDWR);

    unsigned char wvalue = 0x0f, rvalue = 0;
    ret = read(fd, &rvalue, 1);
    printf("read %d %x\n", ret, rvalue);
    ret = write(fd, &wvalue, 1);
    printf("write %d %x\n", ret, wvalue);
    
    sleep(1);
    close(fd);
    
    return 0;
}