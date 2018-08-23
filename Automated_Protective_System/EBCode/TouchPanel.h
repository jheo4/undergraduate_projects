#ifndef __TOUCHPANEL_H__
#define __TOUCHPANEL_H__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // for open/close
#include <fcntl.h>      // for O_RDWR
#include <sys/ioctl.h>  // for ioctl
#include <sys/mman.h>
#include <string.h>
#include <linux/input.h>
#include <linux/fb.h>  

#define	 FBDEV_FILE "/dev/fb0"

#define  INPUT_DEVICE_LIST	"/proc/bus/input/devices"

#define  EVENT_STR	"/dev/input/event"

#define  CH_MAX_BUFF	200

int		ch_screen_width;
int		ch_screen_height;
int		ch_bits_per_pixel;
int		ch_line_length;

#define MAX_TOUCH_X	0x740
#define MAX_TOUCH_Y	0x540

char	ch_eventFullPathName[100];
int	ch_eventnum=2;//마우스(터치이벤트는2번..왜 다른이벤트넣었는지는모르겠)
int	x,y,prex = 0,prey = 0;
int	fb_fd,fp;
struct  fb_var_screeninfo fbvar;
struct  fb_fix_screeninfo fbfix;
unsigned char   *fb_mapped;
int		ch_mem_size;


//draw part;


void ch_closeTouchDev(){
	close(fb_fd);
	close(fp);
}

void ch_readFirstCoordinate(int fd, int* cx , int* cy)
{
	struct input_event event;
	int readSize;
	while(1)
	{
		readSize = read(fd, &event, sizeof(event));

		if ( readSize == sizeof(event) )
		{
//			printf("type :%04X \n",event.type);
//			printf("code :%04X \n",event.code);
//			printf("value:%08X \n",event.value);
			if( event.type == EV_ABS )
			{
				if (event.code == ABS_MT_POSITION_X )
				{
					*cx = event.value*ch_screen_width/MAX_TOUCH_X; 
				}
				else if ( event.code == ABS_MT_POSITION_Y )
				{
					*cy = event.value*ch_screen_height/MAX_TOUCH_Y;
				}
			}
			else if ((event.type == EV_SYN) && (event.code == SYN_REPORT ))
			{
				break;
			}

		}
//		printf("\n");
	}
}

void ch_clearScreen( )//set white whole screen
{
    int		coor_y;
    int		coor_x;
    unsigned long *ptr;
printf("%d\n",fb_mapped);
    for(coor_y = 0; coor_y < ch_screen_height; coor_y++)
    {
        ptr =   (unsigned long *)fb_mapped + ch_screen_width * coor_y ;
        for(coor_x = 0; coor_x < ch_screen_width; coor_x++)
        {
            *ptr++  =   0x000000;
        }
    }
}


void ch_drawRect(unsigned char *fb_mem , int sx , int sy, int ex, int ey,  unsigned long color)
{
    int x,y;
    unsigned long *ptr;

    for(y = sy; y < ey; y++)
    {
        ptr = (unsigned long*)fb_mem + ch_screen_width * y ;

        for (x = sx; x < ex; x++)
        {
            *(ptr + x)  =   color;
        }	
    }

}
#define CUSOR_THICK		10
void ch_drawCoordinate(unsigned char *fb_mem , int cx , int cy, int prex , int prey)
{
	int sx,sy,ex,ey;

	sx = prex - CUSOR_THICK;
	sy = prey - CUSOR_THICK;
	ex = prex + CUSOR_THICK;
	ey = prey + CUSOR_THICK;

	// erase previous cross
	if ( sx < 0 )
		sx = 0;

	if ( sy < 0 )
		sy = 0;

	if (ex >= ch_screen_width)
		ex = ch_screen_width - 1;

	if (ey >= ch_screen_height)
		ey = ch_screen_height -1;

	ch_drawRect(fb_mem, sx, sy, ex, ey, 0x00000000);
//ChangYeol : 여기 왜 draw Rect 두번하는지 모르겠음. 아래에도있음 내일확
	// draw current cross
	sx = cx - CUSOR_THICK;
	sy = cy - CUSOR_THICK;
	ex = cx + CUSOR_THICK;
	ey = cy + CUSOR_THICK;

	// erase previous cross
	if ( sx < 0 )
		sx = 0;

	if ( sy < 0 )
		sy = 0;

	if (ex >= ch_screen_width)
		ex = ch_screen_width - 1;

	if (ey >= ch_screen_height)
		ey = ch_screen_height -1;

	ch_drawRect(fb_mem, sx, sy, ex, ey, 0xFFFFFFFF);
	printf("corX:%d, corY:%d\n",cx,cy);
}


void ch_setTouchPanel()
{
	//2만쓰는데 왜 굳이 이렇게 한줄 모르겠음..;
	sprintf(ch_eventFullPathName,"%s%d",EVENT_STR,ch_eventnum);

	printf("touch input event name:%s\n", ch_eventFullPathName);

	fp = open( ch_eventFullPathName, O_RDONLY);
	if (-1 == fp)
	{
		printf("%s open fail\n",ch_eventFullPathName);
		exit(1);
	}

    if( access(FBDEV_FILE, F_OK) )
    {
        printf("%s: access error\n", FBDEV_FILE);
		close(fp);
        	exit(1);
    }

    if( (fb_fd = open(FBDEV_FILE, O_RDWR)) < 0)
    {
        printf("%s: open error\n", FBDEV_FILE);
		close(fp);
        	exit(1);
    }

    if( ioctl(fb_fd, FBIOGET_VSCREENINFO, &fbvar) )
    {
        printf("%s: ioctl error - FBIOGET_VSCREENINFO \n", FBDEV_FILE);
        ch_closeTouchDev();
    }

    if( ioctl(fb_fd, FBIOGET_FSCREENINFO, &fbfix) )
    {
        printf("%s: ioctl error - FBIOGET_FSCREENINFO \n", FBDEV_FILE);
        ch_closeTouchDev();
    }

    ch_screen_width    =   fbvar.xres;
    ch_screen_height   =   fbvar.yres;
    ch_bits_per_pixel  =   fbvar.bits_per_pixel;
    ch_line_length     =   fbfix.line_length;

    printf("ch_screen_width : %d\n", ch_screen_width);
    printf("ch_screen_height : %d\n", ch_screen_height);
    printf("ch_bits_per_pixel : %d\n", ch_bits_per_pixel);
    printf("ch_line_length : %d\n", ch_line_length);

    ch_mem_size    =   ch_screen_width * ch_screen_height * 4;
    fb_mapped   =   (unsigned char *)mmap(0, ch_mem_size,
                     PROT_READ|PROT_WRITE, MAP_SHARED, fb_fd, 0);
    printf("fb_mape:%d",fb_mapped);
    if (fb_mapped < 0)
    {
        printf("mmap error!\n");
        ch_closeTouchDev();
    }

   //fb_mapped가 메모리 시작주소(3바이트씩 한픽셀당의 데이터를 포인터로 끝까지 가짐)이코드이후로 각위치색지정가능.

}

#endif

