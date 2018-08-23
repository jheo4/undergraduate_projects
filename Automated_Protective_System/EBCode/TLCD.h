#pragma once
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "Global.h"



int IsBusy_TLCD(void)	// busy 이면 TRUE, 아니면 FALSE
{
	unsigned short wdata, rdata;	
	wdata = SIG_BIT_RW;
	write(fdTLCD ,&wdata,2);

	wdata = SIG_BIT_RW | SIG_BIT_E;
	write(fdTLCD ,&wdata,2);

	read(fdTLCD,&rdata ,2);

	wdata = SIG_BIT_RW;
	write(fdTLCD,&wdata,2);

	if (rdata &  BUSY_BIT)
		return TRUE;

	return FALSE;
}
int writeCmd_TLCD(unsigned short cmd)	// cmd의 값을 TLCD로 write..!
{
	unsigned short wdata ;

	if ( IsBusy_TLCD()) return FALSE;	// busy 아닐때만 실행...

	wdata = cmd;
	write(fdTLCD ,&wdata,2);

	wdata = cmd | SIG_BIT_E;
	write(fdTLCD ,&wdata,2);

	wdata = cmd ;
	write(fdTLCD ,&wdata,2);

	return TRUE;
}

int setDDRAMAddr_TLCD(int x , int y)	// 커서의 위치를 설정합니다....
{
	unsigned short cmd = 0;
//	printf("x :%d , y:%d \n",x,y);
	if(IsBusy_TLCD()){
		perror("setDDRAMAddr busy error.\n");
		return FALSE;
	}

	if ( y == 1 )		// 첫번째 라인
	{
		cmd = DDRAM_ADDR_LINE_1 +x;	// 첫번째 라인에 X만큼 이동해서 출력위치 설정
	}
	else if(y == 2 )	// 두번째 라인
	{
		cmd = DDRAM_ADDR_LINE_2 +x;	// 두번째 라인에 X만큼 이동해서 출력위치 설정
	}
	else return FALSE;	// TLCD 지원라인이 최대 2 Line...

	if ( cmd >= 0x80) return FALSE;	
//	printf("setDDRAMAddr w1 :0x%X\n",cmd);
	if (!writeCmd_TLCD(cmd | SET_DDRAM_ADD_DEF))
	{
		perror("setDDRAMAddr error\n");
		return FALSE;
	}
//	printf("setDDRAMAddr w :0x%X\n",cmd|SET_DDRAM_ADD_DEF);
	usleep(1000);
	return TRUE;
}

int displayMode_TLCD(int bCursor, int bCursorblink, int blcd)	// Display mode를 설정합니다... 커서를 보일건지, 깜빡일건지, LCD Dis인지...
{
	unsigned short cmd  = 0;
	if ( bCursor) cmd = DIS_CURSOR;
	if (bCursorblink ) cmd |= DIS_CUR_BLINK;
	if ( blcd ) cmd |= DIS_LCD;

	if (!writeCmd_TLCD(cmd | DIS_DEF)) return FALSE;	// Default 입니다..

	return TRUE;
}

int writeCh_TLCD(unsigned short ch)	// 문자를 씁니다..
{
	unsigned short wdata =0;

	if ( IsBusy_TLCD())
		return FALSE;

	wdata = SIG_BIT_RS | ch;
	write(fdTLCD ,&wdata,2);

	wdata = SIG_BIT_RS | ch | SIG_BIT_E;
	write(fdTLCD ,&wdata,2);

	wdata = SIG_BIT_RS | ch;
	write(fdTLCD ,&wdata,2);
	usleep(1000);
	return TRUE;

}


int setCursorMode_TLCD(int bMove , int bRightDir)	// Cursor 모드 설정... 
{
	unsigned short cmd = MODE_SET_DEF;

	if (bMove)
		cmd |=  MODE_SET_SHIFT;

	if (bRightDir)
		cmd |= MODE_SET_DIR_RIGHT;

	if (!writeCmd_TLCD(cmd))
		return FALSE;
	return TRUE;
}

int functionSet_TLCD(void)
{
	unsigned short cmd = 0x0038; // 5*8 dot charater , 8bit interface , 2 line
	if (!writeCmd_TLCD(cmd)) return FALSE;
	return TRUE;
}

int writeStr_TLCD(char* str)	// 문자열 쓰기...
{
	unsigned char wdata;
	int i;
	for(i =0; i < strlen(str) ;i++ )
	{
		if (str[i] == '_')	// _ 가 스페이스로 대체된다...
			wdata = (unsigned char)' ';
		else
			wdata = str[i];
		writeCh_TLCD(wdata);
	}
	return TRUE;
}


int clearScreen_TLCD(int nline)
{
	int i;
	if (nline == 0)	// 아무것도 없음...
	{
		if(IsBusy_TLCD())
		{	
			perror("clearScreen error\n");
			return FALSE;	
		}
		if (!writeCmd_TLCD(CLEAR_DISPLAY))
			return FALSE;	// clear 실패 

		return TRUE;
	}
	else if (nline == 1)	// 첫번째 라인 Clear
	{	
		setDDRAMAddr_TLCD(0,1);	// x = 0, y = 1
		for(i = 0; i <= COLUMN_NUM ;i++ ) writeCh_TLCD((unsigned char)' '); 	// 공백으로 다 채운다...
		setDDRAMAddr_TLCD(0,1);	// 움직인 커서 다시 처음으로 set

	}
	else if (nline == 2)	// 두번째 라인 Clear
	{	
		setDDRAMAddr_TLCD(0,2);	// x = 0, y = 2
		for(i = 0; i <= COLUMN_NUM ;i++ ) writeCh_TLCD((unsigned char)' '); // 공백으로 다 채운다...
		setDDRAMAddr_TLCD(0,2);	// 움직인 커서 다시 처음으로 set
	}
	return TRUE;
}

void doHelp_TLCD(void)
{
	printf("Usage:\n");
	printf("tlcdtest w line string :=>display the string  at line  , charater  '_' =>' '\n");
	printf(" ex) tlcdtest w 0 cndi_text_test :=>display 'cndi text test' at line 1 \n");
	printf("tlcdtest c on|off blink line column : \n");
	printf(" => cursor set on|off =>1 or 0 , b => blink 1|0 , line column line position \n");
	printf("tlcdtset c  1 1 2 12  :=> display blink cursor at 2 line , 12 column.\n"); 
	printf("tlcdtest r [line] : => clear screen or clear line \n");
	printf("tlcdtest r  : => clear screen \n");
	printf("tlcdtest r 1: => clear line 1 \n");
}


