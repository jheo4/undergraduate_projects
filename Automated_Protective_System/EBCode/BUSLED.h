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



#define LED_ON   1
#define LED_OFF   0
#define MAX_LED_NO      8
#define YOO_BUSLED      "/dev/cnled"



void doHelp_BUSLED(void)
{
   printf("Usage:\n");
   printf("ledtest <ledNo> <1(on)|0(off)>\n");
   printf("ex) ledtest 3 1 ;3th led on\n");
   printf("    ledtest 4 0 ;4th led off\n");
   printf("    ledtest 0 0 ;all led off\n");
   printf("    ledtest 0 1 ;all led on\n");
   printf("Max LedNo =>8 \n");
}




int Yoo_Cnt(int a, int b, int c, int d, int e, int f, int g, int h, int val) // 불빛내기 if쓰기 귀찮다.
{
   int wdata;
   if (val < a)
   {
      wdata = 0x01;
   }
   else if (val < b)
   {
      wdata = 0x03;
   }
   else if (val < c)
   {
      wdata = 0x07;
   }
   else if (val < d)
   {
      wdata = 0x0F;
   }
   else if (val < e)
   {
      wdata = 0x1F;
   }
   else if (val < f)
   {
      wdata = 0x3F;
   }
   else if (val < g)
   {
      wdata = 0x7F;
   }
   else
   {
      wdata = 0xFF;
   }
   return wdata;
}