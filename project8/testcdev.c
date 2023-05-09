#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
  int testdev;
  int i,ret;
  char buf[10] = {0};

  testdev = open("/dev/mycdev",O_RDWR);
  if(testdev == -1){
    printf("connot open file..\n");
    exit(1);
  }
  if((ret = read(testdev,buf,10)) <10){
    printf("read error!\n");
    exit(1);
  }
//  for(i=0;i<10;i++)
    printf("ret %d %s\n",ret ,buf);

  close(testdev);

  return 0;
}