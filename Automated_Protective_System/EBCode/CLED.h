#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>



//////함수

void doHelp_CLED(void)
{
   printf("Usage:Color LED\n");
   printf("cledtest ledindex  red green blue\n");
   printf("color range 0 ~ 255\n");
   printf("ex) cledtest 0  255 255 255 \n");
   printf(" =>0th led is white color \n");
   printf("ex) cledtest 0 255 0 0 \n");
   printf(" =>0th led is Red color \n");
   printf("ledindex : 0 ~ 3 \n");
}




/*
int Yoo_OpenCLED()
{
   
   if (cled_fd < 0)
   {
      perror("driver  open error.\n");
      return 1;
   }
   //printf("index(%d) r(%d) g(%d) b(%d)\n", colorArray[INDEX_LED], colorArray[INDEX_REG_LED], colorArray[INDEX_GREEN_LED], colorArray[INDEX_BLUE_LED]);
   //
   //close(fd);

   return cled_fd;
}*/
