#pragma once
#include "Global.h"
#include "SEVENSEG.h"
#include "DIPSW.h"
/****************************************************************************************************************************




													Woo's Code 




****************************************************************************************************************************/

int Woo_DipSW()
{
    int retvalue;
    int password;
    password = 0xcccc;//패스워드를 받아워서 이 변수에 저장할 예정.
    
    read(fdDip,&retvalue, 16);
    // for Test...
    //printf("Dip SW: %x\n", password);
    if(retvalue == password){
        printf("Keypad Lock is unlocked...\n");
        return TRUE;
    }
    else
        return FALSE;
}


void Woo_SevenSeg(char *argv)
{
    int mode;
    int number;
    char temp[10]; //들어온 char 배열의 값을 임시로 저장해줄 배열
    int resultAtoi; //들어온 char 배열의 정수 값 저장
    int i=0; //배열 Counter
    
    
    mode = MODE_STATIC_DIS;

    //char배열 값을 temp에 임시로 저장
    while(argv[i]!='\0'){
        temp[i] = argv[i];
        i+=1;
    }
    temp[i]='\0';

    resultAtoi = atoi(temp);
    number = resultAtoi;
    
    
    
    // open  driver    
    changemode_SevenSeg(1);//changemode 함수 활성화
    fndDisp_SevenSeg(fdSevnSeg, number, 0, 5);
    changemode_SevenSeg(0);
}