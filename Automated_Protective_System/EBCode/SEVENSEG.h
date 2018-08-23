#pragma once
#include "Global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
/*
        a       
    f       b
        g       
    e       c   
        d       dp


*/



void changemode_SevenSeg(int dir)
{
    if( dir == 1)
    {
        tcgetattr(STDIN_FILENO , &oldt_SevenSeg);
        newt_SevenSeg = oldt_SevenSeg;
        newt_SevenSeg.c_lflag &= ~(ICANON | ECHO );
        tcsetattr(STDIN_FILENO, TCSANOW, &newt_SevenSeg);
    }
    else
    {
        tcsetattr(STDIN_FILENO , TCSANOW, &oldt_SevenSeg);

    }
}

int kbhit_SevenSeg(void)
{
    struct timeval tv;
    fd_set rdfs;

    tv.tv_sec = 0;
    tv.tv_usec = 0;
    
    FD_ZERO(&rdfs);
    FD_SET(STDIN_FILENO , &rdfs);

    select(STDIN_FILENO + 1 , &rdfs , NULL, NULL, &tv);

    return FD_ISSET(STDIN_FILENO , &rdfs);
}

void doHelp_SevenSeg(void)
{
    printf("Usage:\n");
    printf("fndtest option displayTimeSec displaynum [maxcounter]  >\n");
    printf("option   s  : static display  , displaynum range 0 ~ 999999\n");
    printf("option   t  ; time display  \n"); 
    printf("option   c  : counting from 0 to maxcounter .\n");
    printf("ex) fndtest s  1 1234  ; display  '1234' for 1 second\n");
    printf("ex) fndtest t 3 ;display current time for 3 seconds\n");
    printf("ex) fndtest c 2 100 ; display counting number  from  0  to  100  with 2 Second interval.\n");
}


// return 1 => exit  , 0 => success
int fndDisp_SevenSeg(int driverfile, int num , int dotflag,int durationSec)
{
    int cSelCounter,loopCounter;
    int temp , totalCount, i ;
    unsigned short wdata;
    int dotEnable[MAX_FND_NUM];
    int fndChar[MAX_FND_NUM];

    for (i = 0; i < MAX_FND_NUM ; i++ )
    {
        dotEnable[i] = dotflag & (0x1 << i);  
    }
    // if 6 fnd
    temp = num % 1000000;
    fndChar[0]= temp /100000;

    temp = num % 100000;
    fndChar[1]= temp /10000;

    temp = num % 10000;
    fndChar[2] = temp /1000;

    temp = num %1000;
    fndChar[3] = temp /100;

    temp = num %100;
    fndChar[4] = temp /10;

    fndChar[5] = num %10;

    totalCount = durationSec*(1000000 / ONE_SEG_DISPLAY_TIME_USEC);
    //printf("totalcounter: %d\n",totalCount);
    cSelCounter = 0;
    loopCounter = 0;
    while(1)
    {
        wdata = segNum[fndChar[cSelCounter]]  | segSelMask[cSelCounter] ;
        if (dotEnable[cSelCounter])
            wdata |= DOT_OR_DATA;   

        write(driverfile,&wdata,2);

        cSelCounter++;
        if ( cSelCounter >= MAX_FND_NUM )
            cSelCounter = 0;

        usleep(ONE_SEG_DISPLAY_TIME_USEC);

        loopCounter++;
        if ( loopCounter > totalCount )
            break;
        
        if (kbhit_SevenSeg())
        {
            if ( getchar() == (int)'q')
            {
        
                wdata= 0;
                write(driverfile,&wdata,2);
                printf("Exit fndtest\n");
                return 0;
            }

        }
    }

    wdata= 0;
    write(driverfile,&wdata,2);

    return 1;
}

