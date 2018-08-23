#pragma once
#include "Global.h"
#include "BUSLED.h"
#include "KEYPAD.h"
#include "CLED.h"
#include "OLED.h"
#include "BUZZER.h"
#include "TouchPanel.h"
#include "bitmap.h"
/****************************************************************************************************************************




													Yoo's Code 




****************************************************************************************************************************/
int Yoo_BUZZER(int atk);
void YOO_OLED();
int Yoo_Bright(int atk);
int Yoo_KEY();
void Yoo_Busled(int atk, char *val);

int Yoo_BUZZER(int atk)
{
   int buzzerNum;
   if (atk == FLAME_INDEX) // 불꽃감지 // 픽미
   {
      melody(27,24,27,24,20,22,24);
      write(buzzer_fd, 0, 4);
   }
      
   else if (atk == GAS_INDEX) // 가스감지 //비행기
   {
      melody(24, 22, 20, 22, 24, 24, 24);
      write(buzzer_fd, 0, 4);
   }
   else if (atk == SOUND_INDEX) // 사운드감지 //Rudolph the red nosed raindeer
   {
      melody(20, 22, 20, 17, 25, 22, 20);
      write(buzzer_fd, 0, 4);
   }
   else if (atk == VIBE_INDEX) //진동감지 //반짝반짝작은별
   {
      melody(13, 13, 20, 20, 22, 22, 20);
      write(buzzer_fd, 0, 4);
   }
   else if (atk == PIR_INDEX) //적외선감지 // teletoby
   {
      melody(20, 17, 20, 22, 18, 20, 17);
      write(buzzer_fd, 0, 4);
   }
}

int Yoo_Bright(int atk)
{
   unsigned short colorArray[INDEX_MAX];
      
   colorArray[INDEX_LED] = 0;//(unsigned short)atoi(argv[1]);
   if (colorArray[INDEX_LED] > 3)
   {
      doHelp_CLED();
      perror("led index  over range.\n");
      return 1;
   }
   if (atk == FLAME_INDEX){ // 불꽃감지 - 빨강
   colorArray[INDEX_REG_LED] = 120;
   colorArray[INDEX_GREEN_LED] = 0 ;
   colorArray[INDEX_BLUE_LED] = 0;
	}
   else if (atk == GAS_INDEX){ // 가스감지 - 초록
      colorArray[INDEX_REG_LED] = 0;
   colorArray[INDEX_GREEN_LED] = 120 ;
   colorArray[INDEX_BLUE_LED] = 0;
	}
   else if (atk == SOUND_INDEX){ // 사운드감지 - 파랑
	     colorArray[INDEX_REG_LED] = 0;
	   colorArray[INDEX_GREEN_LED] = 0 ;
	   colorArray[INDEX_BLUE_LED] = 120;
	}
   else if (atk == VIBE_INDEX){ // 진동감지 - 노랑
      colorArray[INDEX_REG_LED] = 120;
   colorArray[INDEX_GREEN_LED] = 120 ;
   colorArray[INDEX_BLUE_LED] = 0;
   }
   else if (atk == PIR_INDEX){ // 적외선감지 - 흰색
      colorArray[INDEX_REG_LED] = 120;
   colorArray[INDEX_GREEN_LED] = 120 ;
   colorArray[INDEX_BLUE_LED] = 120;
   }else{
   	  colorArray[INDEX_REG_LED] = 0;
   colorArray[INDEX_GREEN_LED] = 0 ;
   colorArray[INDEX_BLUE_LED] = 0;
   }
   write(cled_fd, &colorArray, 6);
   
}


int Yoo_KEY() // 13번으로 시작하고 14로끝냄 1234가 비밀번호
{
	int pw[5] = { 0, };
	int i = 0; int j=0;
	int predata = 0;
	int rdata = 0;
	int keyFlag = 0;

	ch_setTouchPanel();//code for EB/when testing not in EB, hide this code;
	//clear(set white all) 
 
	ch_clearScreen();//set black background TouchPanel.h에 있는 함수들.

   
	ch_drawObj title;
	ch_drawObj pwd_blnk;
	ch_drawObj pwd_fill;
  int cur_xposition=450;

	title.xposition=300;
	title.yposition=150;
	title.filename="btn/password_title.bmp";
   
   //printf("x,y:%d,%d\n",titlexposition,tem->yposition);
	Yoo_imgDraw(&title);

	pwd_fill.xposition=cur_xposition;
	pwd_fill.yposition=450;
	pwd_fill.filename="btn/pwd_fill.bmp";

   pwd_blnk.yposition=450;
   pwd_blnk.filename="btn/pwd_blnk.bmp";
	
	for(i=0;i<4;i++){
		pwd_blnk.xposition=cur_xposition+i*100;
		Yoo_imgDraw(&pwd_blnk);	
	}

	while (1){
		read(key_fd, &rdata, 4);
		if (rdata == 13){ keyFlag = 1; printf("Buffer On\n");}
		if(keyFlag == 1){
			for(i = 0 ; i < 4 ;){
				read(key_fd, &rdata, 4);
				
				if( predata != rdata && rdata!=0 ){
               pwd_fill.xposition=cur_xposition+i*100;
            Yoo_imgDraw(&pwd_fill);
					predata = rdata;
					pw[i] = rdata;
					i++;
				}
			}
			if (pw[0] == 13 && pw[1] == 1 && pw[2] == 2 && pw[3] == 3){ 
				///////////////////// for test 
				printf("Bunker is operated\n");
				return TRUE;
			}else{
				if(pw[4]!=0) for(j = 0 ; j < 5 ; j ++ ) pw[j]=0;
				keyFlag = 0;
				printf("incorret Password\n");
            for(i=0;i<4;i++){
               pwd_blnk.xposition=cur_xposition+i*100;
               Yoo_imgDraw(&pwd_blnk); 
            }
			}
		}
	}
	return 0;
}

void Yoo_Busled(int atk, char *val)
{
   int value = atoi(val);
   int wdata;
   wdata = 0x00;
   write(busled_fd, &wdata, 4);
   // for Testing ... 
   //value = 80;
   // for Testing ...
   if (atk == FLAME_INDEX) // 불꽃감지
   {
      wdata = Yoo_Cnt(60, 70, 80, 90, 100, 110, 120, 130, value);
   }
   else if (atk == GAS_INDEX) // 가스감지
   {
      wdata = Yoo_Cnt(850, 870, 890, 900, 920, 960, 1000, 1100, value);
   }
   else if (atk == SOUND_INDEX) // 사운드감지
   {
      wdata = Yoo_Cnt(550, 600, 700, 800, 900, 950, 1000, 1100, value);
   }
   else if (atk == VIBE_INDEX) //진동감지
   { 
      wdata = Yoo_Cnt(700, 750, 800, 850, 900, 950, 1000, 1100, value);
   }
   else if (atk == PIR_INDEX) //적외선감지
   {
      wdata = 0xFF;
   }else{
   	wdata = 0x00;
   }

   write(busled_fd, &wdata, 4);
}

void Yoo_imgDraw(ch_drawObj* tem){  
   int i,j;
	char *pData;
	char *data;
	unsigned long *ptr;	
   
 
		ch_read_bmp(tem->filename, &pData, &data, &(tem->xlength), &(tem->ylength));
		
		printf("close bmp pdata\n");	

	//	int a=1;
		int b,g,r;
		long p;
		long pixel;

		for (j = 0; j<tem->ylength; j++)
		{
		
//------------------------------------------------------------------------------------
			ptr=(unsigned long*) fb_mapped +ch_screen_width*((tem->yposition+tem->ylength)-j);
//------------------------------------------------------------------------------------------------------------
      
		
			p = j*3*(tem->xlength);

			for (i = 0; i<tem->xlength; i++)
			{
				
				b = *(data + (p + i * 3));
				g = *(data + (p + i * 3 + 1));
				r = *(data + (p + i * 3 + 2));

				pixel = ((r << 16) | (g << 8) | b);
				*(ptr+tem->xposition+i)=pixel;//리눅스에선 주석할것
			}
		}

		printf("\nafter draw\n");

}


void YOO_OLED(){
   reset();//1
   Init();//2
   if(caOPcode[2]=='F'&&caOPcode[3]=='F'){
      imageLoading("Flame.img");//3
   }
   else if(caOPcode[2]=='S'&&caOPcode[3]=='S'){
      imageLoading("Sound.img");//3
   }
   else if(caOPcode[2]=='V'&&caOPcode[3]=='V'){
      imageLoading("Vib.img");//3
   }else if(caOPcode[2]=='G'&&caOPcode[3]=='G'){
      imageLoading("Gas.img");//3
   }
   else{
      imageLoading("Logo.img");//3
   }   
}