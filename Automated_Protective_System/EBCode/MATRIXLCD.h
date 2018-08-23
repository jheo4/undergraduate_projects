#pragma once
#include "Global.h"
#define DRIVER_DOT		"/dev/cnmled"	// Dot matrix Path
#define MAX_COLUMN_NUM	5
#define ONE_LINE_TIME_U 	1000

const unsigned short NumData[10][MAX_COLUMN_NUM] =	// dot matrix 10*7
{
	{ 0xfe00,0xfd7F,0xfb41,0xf77F,0xef00 }, // 0
	{ 0xfe00,0xfd42,0xfb7F,0xf740,0xef00 }, // 1
	{ 0xfe00,0xfd79,0xfb49,0xf74F,0xef00 }, // 2
	{ 0xfe00,0xfd49,0xfb49,0xf77F,0xef00 }, // 3
	{ 0xfe00,0xfd0F,0xfb08,0xf77F,0xef00 }, // 4
	{ 0xfe00,0xfd4F,0xfb49,0xf779,0xef00 }, // 5
	{ 0xfe00,0xfd7F,0xfb49,0xf779,0xef00 }, // 6
	{ 0xfe00,0xfd07,0xfb01,0xf77F,0xef00 }, // 7
	{ 0xfe00,0xfd7F,0xfb49,0xf77F,0xef00 }, // 8
	{ 0xfe00,0xfd4F,0xfb49,0xf77F,0xef00 }  // 9	// seven segment¼ýÀÚ 
};
const unsigned short Jin_DOTLED[MAX_COLUMN_NUM]={0x107F,0x117F, 0x207F, 0x217F, 0x227F};
void doHelp_Dot(void)
{
	printf("Usage:\n");
	printf("mledtest option displayTime number\n");
	printf("exit 'q' \n");
	printf("ex) mledtest s 3 71 ; display '71' for 3 Seconds\n");
	printf("ex) mledtest c 3 71 ; counting  from 0 to 71  with 3 Seconds interval.\n");

}



void changemode_Dot(int dir)
{
	if (dir == 1)
	{
		tcgetattr(STDIN_FILENO, &oldt_Matrix);
		newt_Matrix = oldt_Matrix;
		newt_Matrix.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &newt_Matrix);
	}
	else
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &oldt_Matrix);
	}
}


int kbhit_Dot(void)
{
	struct timeval tv;
	fd_set rdfs;

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	FD_ZERO(&rdfs);
	FD_SET(STDIN_FILENO, &rdfs);

	select(STDIN_FILENO + 1, &rdfs, NULL, NULL, &tv);

	return FD_ISSET(STDIN_FILENO, &rdfs);
}

int displayDotLed_Dot(int driverfile, int num, int timeS)
{
	int cSelCounter, loopCounter;
	int highChar, lowChar;
	int temp, totalCount;
	unsigned short wdata[2];

	temp = num % 100;

	highChar = temp / 10;	// x-
	lowChar = temp % 10;	// -x


	totalCount = timeS*(1000000 / ONE_LINE_TIME_U);
	//printf("totalcounter: %d\n", totalCount);
	cSelCounter = 0;
	loopCounter = 0;
	while (1)
	{
		// high byte display 
		//wdata[0] = NumData[highChar][cSelCounter];	// x-
		wdata[0] = Jin_DOTLED[cSelCounter];	// x-
		// low byte display 
		wdata[1] = Jin_DOTLED[cSelCounter];	// -x

		write(driverfile, (unsigned char*)wdata, 4);

		cSelCounter++;
		if (cSelCounter >= (MAX_COLUMN_NUM - 1))
			cSelCounter = 1;

		//usleep(ONE_LINE_TIME_U);

		loopCounter++;
		if (loopCounter > totalCount)
			break;

		if (kbhit_Dot())
		{
			if (getchar() == (int)'q')
			{

				wdata[0] = 0;
				wdata[1] = 0;
				write(driverfile, (unsigned char*)wdata, 4);
				//printf("Exit mledtest\n");
				return 0;
			}

		}

	}
	wdata[0] = 0;
	wdata[1] = 0;
	write(driverfile, (unsigned char*)wdata, 4);

	return 1;
}


void displayDotLed_Jin(int driverfile, int _iKind)
{	
	int cSelCounter, loopCounter;
	int highChar, lowChar;
	int temp, totalCount;
	unsigned short wdata[2];

	if(_iKind == 1){
		cSelCounter = 0;
		loopCounter = 0;
		while (1)
		{
			// high byte display 
			//wdata[0] = NumData[highChar][cSelCounter];	// x-
			wdata[0] = Jin_DOTLED[cSelCounter];	// x-
			// low byte display 
			wdata[1] = Jin_DOTLED[cSelCounter];	// -x

			write(driverfile, (unsigned char*)wdata, 4);

			cSelCounter++;
			if (cSelCounter >= (MAX_COLUMN_NUM - 1))
				cSelCounter = 1;

			//usleep(ONE_LINE_TIME_U);

			loopCounter++;
			if (loopCounter > 80)
				break;

		}
	}else{
		wdata[0] = 0;
		wdata[1] = 0;
		write(driverfile, (unsigned char*)wdata, 4);
	}
	return ;
}