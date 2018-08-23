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



void doHelp_DipSW(void)
{
    printf("Usage:\n");
    printf("dipsw \n");
    printf("return  int  (0~ 15 bit are valid). \n");
}


