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



/*
키패드
1    2    3     4
5    6    7     8
9    10   11    12
시작 끝  사용X  사용X

*/

void doHelp_KEY(void)
{
   printf("Usage:\n");
   printf("key \n");
   printf("return   0 ~15 (push button nubmer)\n");
}


