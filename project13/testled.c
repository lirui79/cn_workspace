#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>


int main(void)
{
    int fd, ret, i ;
    char devName[32] = {0};
    for (i = 0; i < 4; ++i) {
          sprintf(devName,"/dev/LED%d", i + 1);
          fd = open(devName,O_RDWR);
	  //fd = open("/dev/tiny4412_led",O_RDWR);
	  unsigned int wvalue = 0x12323, rvalue = 0;
	    ret = read(fd, &rvalue, sizeof(unsigned int));
	    printf("read %d %x\n", ret, rvalue);
	    ret = write(fd, &wvalue, sizeof(unsigned int));
	    printf("write %d %x\n", ret, wvalue);
	    
	    sleep(1);
	    close(fd);
    }
    
    return 0;
}
