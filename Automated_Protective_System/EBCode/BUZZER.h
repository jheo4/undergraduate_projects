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

void melody(int a, int b, int c, int d, int e, int f, int g);

void melody(int a, int b, int c, int d, int e, int f, int g){
	int buzzerNum;
   buzzerNum = a;
   write(buzzer_fd, &buzzerNum, 4);
   usleep(500000);
   buzzerNum = b;
   write(buzzer_fd, &buzzerNum, 4);
   usleep(500000);
   buzzerNum = c;
   write(buzzer_fd, &buzzerNum, 4);
   usleep(500000);
   buzzerNum = d;
   write(buzzer_fd, &buzzerNum, 4);
   usleep(500000);
   buzzerNum = e;
   write(buzzer_fd, &buzzerNum, 4);
   usleep(500000);
   buzzerNum = f;
   write(buzzer_fd, &buzzerNum, 4);
   usleep(500000);
   buzzerNum = g;
   write(buzzer_fd, &buzzerNum, 4);
   usleep(500000);

   write(buzzer_fd, 0, 4);
}