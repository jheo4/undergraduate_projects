#pragma once
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
#include "Global.h"

unsigned long simple_strtoul(char *cp, char **endp, unsigned int base)
{
   unsigned long result = 0, value;

   if (*cp == '0') {
      cp++;
      if ((*cp == 'x') && isxdigit(cp[1])) {
         base = 16;
         cp++;
      }
      if (!base) {
         base = 8;
      }
   }
   if (!base) {
      base = 10;
   }
   while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp - '0' : (islower(*cp)
      ? toupper(*cp) : *cp) - 'A' + 10) < base) {
      result = result*base + value;
      cp++;
   }
   if (endp)
      *endp = (char *)cp;
   return result;
}

unsigned long read_hex(const char* str){
   char addr[128];
   strcpy(addr, str);
   return simple_strtoul(addr, NULL, 16);
}


int reset(void)
{
   unsigned short wdata;

   wdata = RST_BIT_MASK;
   write(oled_fd, &wdata, 2);
   usleep(2000);
   wdata = DEFAULT_MASK;
   write(oled_fd, &wdata, 2);
   return TRUE;
}

int writeCmd(int size, unsigned short* cmdArr)
{
   int i;
   unsigned short wdata;

   //printf("wCmd : [0x%02X]",cmdArr[0]);
   //wdata = CS_BIT_MASK;
   //write(oled_fd,&wdata,2);

   wdata = CS_BIT_MASK & DC_BIT_MASK;
   write(oled_fd, &wdata, 2);

   wdata = CS_BIT_MASK & DC_BIT_MASK & WD_BIT_MASK;
   write(oled_fd, &wdata, 2);

   wdata = CS_BIT_MASK & DC_BIT_MASK & WD_BIT_MASK & (cmdArr[0] | 0xFF00);
   write(oled_fd, &wdata, 2);

   wdata = CS_BIT_MASK & DC_BIT_MASK & (cmdArr[0] | 0xFF00);
   write(oled_fd, &wdata, 2);

   wdata = CS_BIT_MASK & (cmdArr[0] | 0xFF00);
   write(oled_fd, &wdata, 2);

   for (i = 1; i < size; i++)
   {
      //   wdata = CS_BIT_MASK ;
      //   write(oled_fd,&wdata,2);

      //   wdata = CS_BIT_MASK ;
      //   write(oled_fd,&wdata,2);

      wdata = CS_BIT_MASK & WD_BIT_MASK;
      write(oled_fd, &wdata, 2);

      wdata = CS_BIT_MASK & WD_BIT_MASK & (cmdArr[i] | 0xFF00);
      write(oled_fd, &wdata, 2);

      wdata = CS_BIT_MASK & (cmdArr[i] | 0xFF00);
      write(oled_fd, &wdata, 2);

      //   wdata = CS_BIT_MASK & (cmdArr[i] | 0xFF00);
      //   write(oled_fd,&wdata,2);
      //   printf("[0x%02X]",cmdArr[i]);

   }
   wdata = DEFAULT_MASK;
   write(oled_fd, &wdata, 2);
   //printf("\n");
   return TRUE;
}

int writeData(int size, unsigned char* dataArr)
{
   int i;
   unsigned short wdata;

   //wdata = CS_BIT_MASK;
   //write(oled_fd,&wdata,2);

   wdata = CS_BIT_MASK & DC_BIT_MASK;
   write(oled_fd, &wdata, 2);

   //wdata = CS_BIT_MASK & DC_BIT_MASK & WD_BIT_MASK ;
   //write(oled_fd,&wdata,2);

   wdata = CS_BIT_MASK & DC_BIT_MASK & WD_BIT_MASK & (CMD_WRITE_RAM | 0xFF00);
   write(oled_fd, &wdata, 2);

   wdata = CS_BIT_MASK & DC_BIT_MASK & (CMD_WRITE_RAM | 0xFF00);
   write(oled_fd, &wdata, 2);

   wdata = CS_BIT_MASK &  (CMD_WRITE_RAM | 0xFF00);
   write(oled_fd, &wdata, 2);

   for (i = 0; i < size; i++)
   {
      wdata = CS_BIT_MASK & WD_BIT_MASK;
      write(oled_fd, &wdata, 2);

      wdata = CS_BIT_MASK & WD_BIT_MASK & ((unsigned char)dataArr[i] | 0xFF00);
      write(oled_fd, &wdata, 2);

      wdata = CS_BIT_MASK & ((unsigned char)dataArr[i] | 0xFF00);
      write(oled_fd, &wdata, 2);


   }
   wdata = DEFAULT_MASK;
   write(oled_fd, &wdata, 2);

   return TRUE;

}

int readData(int size, unsigned short* dataArr)
{

   int i;
   unsigned short wdata;

   wdata = CS_BIT_MASK & DC_BIT_MASK;
   write(oled_fd, &wdata, 2);

   wdata = CS_BIT_MASK & DC_BIT_MASK & (CMD_READ_RAM | 0xFF00);
   write(oled_fd, &wdata, 2);

   wdata = CS_BIT_MASK & DC_BIT_MASK & WD_BIT_MASK &(CMD_READ_RAM | 0xFF00);
   write(oled_fd, &wdata, 2);

   wdata = CS_BIT_MASK & DC_BIT_MASK & (CMD_READ_RAM | 0xFF00);
   write(oled_fd, &wdata, 2);

   wdata = CS_BIT_MASK &  (CMD_READ_RAM | 0xFF00);
   write(oled_fd, &wdata, 2);


   for (i = 0; i < size; i++)
   {
      //wdata = CS_BIT_MASK ;
      //write(oled_fd,&wdata,2);

      wdata = CS_BIT_MASK;
      write(oled_fd, &wdata, 2);

      wdata = CS_BIT_MASK & RD_BIT_MASK;
      write(oled_fd, &wdata, 2);

      wdata = CS_BIT_MASK & RD_BIT_MASK;
      write(oled_fd, &wdata, 2);

      wdata = CS_BIT_MASK;
      write(oled_fd, &wdata, 2);

      read(oled_fd, &dataArr[i], 2);

      //wdata = CS_BIT_MASK ;
      //write(oled_fd,&wdata,2);

   }
   wdata = DEFAULT_MASK;
   write(oled_fd, &wdata, 2);

   return TRUE;
}

int setAddressDefalut(void)
{
   unsigned short  cmd[3];
   cmd[0] = CMD_SET_COLUMN_ADDR;
   cmd[1] = 0;
   cmd[2] = 127;
   writeCmd(3, cmd);

   cmd[0] = CMD_SET_ROW_ADDR;
   cmd[1] = 0;
   cmd[2] = 127;
   writeCmd(3, cmd);

   return TRUE;
}

// to send cmd  , must unlock
int setCmdLock(int bLock)
{
   unsigned short  cmd[3];

   cmd[0] = CMD_LOCK;
   if (bLock)
   {
      cmd[1] = 0x16; // lock
      writeCmd(2, cmd);

   }
   else
   {
      cmd[1] = 0x12; // lock
      writeCmd(2, cmd);

      // A2,B1,B3,BB,BE accessible
      cmd[1] = 0xB1;
      writeCmd(2, cmd);
   }
   return TRUE;
}

int imageLoading(char* fileName)
{
   int imgfile;
   unsigned char* data = NULL;
   int  width, height;

   imgfile = open(fileName, O_RDONLY);
   if (imgfile < 0)
   {
      printf("imageloading(%s)  file is not exist . err.\n", fileName);
      return FALSE;
   }
   setCmdLock(FALSE);


   read(imgfile, &width, sizeof(unsigned char));
   read(imgfile, &height, sizeof(unsigned char));

   data = malloc(128 * 128 * 3);

   read(imgfile, data, 128 * 128 * 3);

   close(imgfile);

   writeData(128 * 128 * 3, data);

   setCmdLock(TRUE);
   return TRUE;
}




int Init(void)
{
   unsigned short wdata[10];
   unsigned char  wcdata[10];
   int i, j;
   wdata[0] = 0xFD;
   wdata[1] = 0x12;
   writeCmd(2, wdata);


   wdata[0] = 0xFD;
   wdata[1] = 0xB1;
   writeCmd(2, wdata);

   wdata[0] = 0xAE;
   writeCmd(1, wdata);

   wdata[0] = 0xB3;
   wdata[1] = 0xF1;
   writeCmd(2, wdata);

   wdata[0] = 0xCA;
   wdata[1] = 0x7F;
   writeCmd(2, wdata);

   wdata[0] = 0xA2;
   wdata[1] = 0x00;
   writeCmd(2, wdata);

   wdata[0] = 0xA1;
   wdata[1] = 0x00;
   writeCmd(2, wdata);

   wdata[0] = 0xA0;
   wdata[1] = 0xB4;
   writeCmd(2, wdata);

   wdata[0] = 0xAB;
   wdata[1] = 0x01;
   writeCmd(2, wdata);

   wdata[0] = 0xB4;
   wdata[1] = 0xA0;
   wdata[2] = 0xB5;
   wdata[3] = 0x55;
   writeCmd(4, wdata);

   wdata[0] = 0xC1;
   wdata[1] = 0xC8;
   wdata[2] = 0x80;
   wdata[3] = 0xC8;
   writeCmd(4, wdata);

   wdata[0] = 0xC7;
   wdata[1] = 0x0F;
   writeCmd(2, wdata);

   // gamma setting 
   writeCmd(64, gamma);


   wdata[0] = 0xB1;
   wdata[1] = 0x32;
   writeCmd(2, wdata);

   wdata[0] = 0xB2;
   wdata[1] = 0xA4;
   wdata[2] = 0x00;
   wdata[3] = 0x00;
   writeCmd(4, wdata);

   wdata[0] = 0xBB;
   wdata[1] = 0x17;
   writeCmd(2, wdata);

   wdata[0] = 0xB6;
   wdata[1] = 0x01;
   writeCmd(2, wdata);

   wdata[0] = 0xBE;
   wdata[1] = 0x05;
   writeCmd(2, wdata);

   wdata[0] = 0xA6;
   writeCmd(1, wdata);


   for (i = 0; i < 128; i++)
   {
      for (j = 0; j < 128; j++)
      {
         wcdata[0] = 0x3F;
         wcdata[1] = 0;
         wcdata[2] = 0;
         writeData(3, wcdata);
      }

   }

   wdata[0] = 0xAF;
   writeCmd(1, wdata);
   return TRUE;
}

