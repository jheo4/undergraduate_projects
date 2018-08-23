#pragma once
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>     // for open/close
#include <sys/ioctl.h>  // for ioctl
#include <sys/mman.h>
#include <string.h>
#include <linux/input.h>
#include <linux/fb.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BAUDRATE B9600      /* baud rate */ 
#define ARDUINO_DEV "/dev/ttyACM0"   /* arduino device */ 
#define FALSE 0
#define TRUE 1
#define SIZE_OF_BUFF 255

#define FLAME_INDEX 0
#define GAS_INDEX 1
#define SOUND_INDEX 2
#define VIBE_INDEX 3
#define PIR_INDEX 4

#define SUCCESS		0
#define FAIL		1
#define TLCD_DRIVER_NAME		"/dev/cntlcd"
#define CLEAR_DISPLAY		0x0001
#define CURSOR_AT_HOME		0x0002
// Entry Mode set 
#define MODE_SET_DEF		0x0004
#define MODE_SET_DIR_RIGHT	0x0002
#define MODE_SET_SHIFT		0x0001
// Display on off
#define DIS_DEF				0x0008
#define DIS_LCD				0x0004
#define DIS_CURSOR			0x0002
#define DIS_CUR_BLINK		0x0001
// shift
#define CUR_DIS_DEF			0x0010
#define CUR_DIS_SHIFT		0x0008
#define CUR_DIS_DIR			0x0004
// set DDRAM  address 
#define SET_DDRAM_ADD_DEF	0x0080
// read bit
#define BUSY_BIT			0x0080
#define DDRAM_ADD_MASK		0x007F
#define DDRAM_ADDR_LINE_1	0x0000
#define DDRAM_ADDR_LINE_2	0x0040
#define SIG_BIT_E			0x0400
#define SIG_BIT_RW			0x0200
#define SIG_BIT_RS			0x0100
#define CMD_TXT_WRITE		0
#define CMD_CURSOR_POS		1
#define CMD_CEAR_SCREEN		2
#define LINE_NUM			2
#define COLUMN_NUM			16	
#define RST_BIT_MASK   0xEFFF      
#define CS_BIT_MASK      0xF7FF
#define DC_BIT_MASK      0xFBFF
#define WD_BIT_MASK      0xFDFF
#define RD_BIT_MASK      0xFEFF
#define DEFAULT_MASK   0xFFFF
#define CMD_SET_COLUMN_ADDR      0x15
#define CMD_SET_ROW_ADDR      0x75
#define CMD_WRITE_RAM         0x5C
#define CMD_READ_RAM         0x5D
#define CMD_LOCK            0xFD
/////////////////////////////////////////////////////////////////////// Seven Seg...
#define WOO_DRIVER     "/dev/cnfnd"
#define MAX_FND_NUM     6
#define  DOT_OR_DATA    0x80
#define ONE_SEG_DISPLAY_TIME_USEC   1000
#define MODE_STATIC_DIS     0
#define MODE_TIME_DIS       1
#define MODE_COUNT_DIS      2
#define YOO_CLED      "/dev/cncled"
#define INDEX_LED      0
#define INDEX_REG_LED      1
#define INDEX_GREEN_LED      2
#define INDEX_BLUE_LED      3
#define INDEX_MAX         4
//////////////////////////////////////////////////thread printing duration
#define PRINT_DURATION 2
/////////////////////////////////////////////////////////////////////// KEY Pad
#define YOO_KEY      "/dev/cnkey"
/////////////////////////////////////////////////////////////////////// Dip SW
#define DRIVER_DIP "/dev/cndipsw"
/////////////////////////////////////////////////////////////////////// OLED 
#define DRIVER_OLED      "/dev/cnoled"
#define YOO_BUZZER      "/dev/cnbuzzer"
#define MAX_BUZZER_NUMBER      36

#define OPEN 1
#define CLOSE 2 
/////////////////////////////////////////////////////////////////////// Main

struct termios oldtio, newtio;
static struct termios oldt_Matrix, newt_Matrix;

int iSizeOfString;
char caBuf1[SIZE_OF_BUFF];
char caOPcode[5]={0,};	// Bunker state & Ex or In & Sensor indentity
char caValue[10]={'\0',};	// Sensor value

volatile int STOP = FALSE;


/////////////////////////////////////////////////////////////////////// File Stream...
static int key_fd;
static int busled_fd;
static int fdTLCD ;
static int cled_fd;
static int fdDip;
static int fdSevnSeg;
static int fdDOT;
static int fArduino;
static int oled_fd;
static int buzzer_fd;

/////////////////////////////////////////////////////////////////////// TLCD
char strWtext[2][COLUMN_NUM];	// 17 column...

static struct termios oldt_SevenSeg, newt_SevenSeg;
const unsigned short segNum[10] =
{
    0x3F, // 0
    0x06,
    0x5B,
    0x4F,
    0x66,
    0x6D,
    0x7D,
    0x27,
    0x7F,
    0x6F  // 9
};
const unsigned short segSelMask[MAX_FND_NUM] = 
{
    0xFE00,
    0xFD00,
    0xFB00,
    0xF700,
    0xEF00,
    0xDF00
};

/////////////////////////////////////////////////////////////////////// OLED
static unsigned short gamma[64] =
{
   0xB8,
   0x02, 0x03, 0x04, 0x05,
   0x06, 0x07, 0x08, 0x09,
   0x0A, 0x0B, 0x0C, 0x0D,
   0x0E, 0x0F, 0x10, 0x11,
   0x12, 0x13, 0x15, 0x17,
   0x19, 0x1B, 0x1D, 0x1F,
   0x21, 0x23, 0x25, 0x27,
   0x2A, 0x2D, 0x30, 0x33,
   0x36, 0x39, 0x3C, 0x3F,
   0x42, 0x45, 0x48, 0x4C,
   0x50, 0x54, 0x58, 0x5C,
   0x60, 0x64, 0x68, 0x6C,
   0x70, 0x74, 0x78, 0x7D,
   0x82, 0x87, 0x8C, 0x91,
   0x96, 0x9B, 0xA0, 0xA5,
   0xAA, 0xAF, 0xB4
};

#define MAX_INPUT_THREAD 2
#define MAX_OUTPUT_THREAD 7

		// Input Index
#define DIP_N_KEYPAD	0
#define TOUCH			1

		// Output Index
#define DOT	0
#define SEVENSEG 		1
#define BUZ 			2
#define SCR 			3
#define TLCD_TH			4
#define BUSLED			5
#define COLLED			6
pthread_t ArduinoThread;
pthread_t InputThread[MAX_INPUT_THREAD];
pthread_t Sevenseg_thd;
pthread_t Buzzer_thd;
pthread_t TouchCam_thd;
pthread_t Signal_thd;
pthread_t Sock_thd;

int iAttack;
sem_t Semaphore1;

int iBunkerState = OPEN;
int iOpenSignal = 0 ;
int iCloseSignal = 0 ;