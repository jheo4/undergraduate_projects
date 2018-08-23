#include "Mutal.h"


CMutal::CMutal(void){
	rtPoint.top = 0;
	rtPoint.bottom = 0;
	rtPoint.left = 0;
	rtPoint.right = 0;
	iState=0;	
	iHP=0;
	iAP=0;
	bLife=FALSE; bDying = FALSE;
}
void CMutal::CreateMutal(){
	rtPoint.top = 0;
	rtPoint.bottom = MUTAL_H;

	rtPoint.left = rand()%(MAP_X-MUTAL_W);
	rtPoint.right = rtPoint.left+MUTAL_W;
	iHP = 160; iAP = 9;
	bLife = TRUE; bDying = FALSE;
	iState =1;
}
void CMutal::DeleteMutal(){bLife = FALSE; bDying = FALSE;}

CMutal::~CMutal(void)
{
}

void CMutal::Draw(HDC &hdc, HDC &FrontDC, HDC &BackDC, HINSTANCE hInstance, HBITMAP &BackBitmap){
	OldBitmap = (HBITMAP)SelectObject(FrontDC, BackBitmap);
	if(bDying){
		Motion = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_ZERGBOOM));	// Mutal Dead img!
		SelectObject(BackDC, Motion);
		BitBlt(FrontDC, 0, 0, 35, 35, BackDC, 0, 0, SRCCOPY);
		TransparentBlt(hdc, rtPoint.left, rtPoint.top, ZERGBOOM_W, ZERGBOOM_H, FrontDC, 0, 0, ZERGBOOM_W, ZERGBOOM_H, RGB(255,255,255));
	}
	else{
		switch(iState){
		case Motion1:
			Motion = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_MUTAL1));  break;
		case Motion2:
			Motion = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_MUTAL2));  break;
		case Motion3:
			Motion = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_MUTAL3));  break;
		}
		SelectObject(BackDC, Motion);
		BitBlt(FrontDC, 0, 0, MUTAL_W, MUTAL_H, BackDC, 0, 0, SRCCOPY);
		TransparentBlt(hdc, rtPoint.left, rtPoint.top, MUTAL_W, MUTAL_H, FrontDC, 0, 0, MUTAL_W, MUTAL_H, RGB(255,255,255));
	}
	SelectObject(FrontDC, OldBitmap);
	DeleteObject(Motion);
	//if(bDying)
	//	DeleteMutal();	// Mutal Dead img!
};
void CMutal::Move(){
	if(!bDying){
		rtPoint.top += 4;
		rtPoint.bottom += 4;
		if(rtPoint.bottom > MAP_Y)
			DeleteMutal();
	}
};
