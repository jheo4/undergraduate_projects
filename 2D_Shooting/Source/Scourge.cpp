#include "Scourge.h"


CScourge::CScourge(void)
{
	rtPoint.top = 0;
	rtPoint.bottom = 0;
	rtPoint.left = 0;
	rtPoint.right = 0;
	iState=0;	
	iHP=0;
	iAP=0;
	bLife=FALSE;
}


CScourge::~CScourge(void)
{
}


void CScourge::CreateScourge(){
	rtPoint.top = 0;
	rtPoint.bottom = SCOURGE_H;
	iMove = rand()%150;
	iDistance = 0;

	rtPoint.left = rand()%(MAP_X-SCOURGE_W);
	rtPoint.right = rtPoint.left+SCOURGE_W;
	iHP = 25; iAP = 20;
	bLife = TRUE; bDying = FALSE;
}
void CScourge::Draw(HDC &hdc, HDC &FrontDC, HDC &BackDC, HINSTANCE hInstance, HBITMAP &BackBitmap){
	OldBitmap = (HBITMAP)SelectObject(FrontDC, BackBitmap);
	if(bDying){
		Motion = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_ZERGBOOM)); // Dying
		SelectObject(BackDC, Motion);
		BitBlt(FrontDC, 0, 0, 35, 35, BackDC, 0, 0, SRCCOPY);
		TransparentBlt(hdc, rtPoint.left-10, rtPoint.top-10, ZERGBOOM_W, ZERGBOOM_H, FrontDC, 0, 0, 35, 35, RGB(255,255,255));
	}else{
		Motion = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_SCOURGE));
		SelectObject(BackDC, Motion);
		BitBlt(FrontDC, 0, 0, SCOURGE_W, SCOURGE_H, BackDC, 0, 0, SRCCOPY);
		TransparentBlt(hdc, rtPoint.left, rtPoint.top, SCOURGE_W, SCOURGE_H, FrontDC, 0, 0, SCOURGE_W, SCOURGE_H, RGB(255,255,255));
	}	
	SelectObject(FrontDC, OldBitmap);
	DeleteObject(Motion);
	if(bDying)
		iAP=0;	// Mutal Dead img!
}
void CScourge::Move(){	// the target is Battle!
	if(!bDying){
		rtPoint.top += 5;
		rtPoint.bottom += 5;
		switch(iState){
		case 0:
			if(iDistance<iMove){
				if(rtPoint.left>0){
					rtPoint.left--; rtPoint.right--;
					iDistance++;
				}
			}
			if(iDistance==iMove)
				iState=1;
			break;
		case 1:
			if(iDistance>0){
				if(rtPoint.left+SCOURGE_W<MAP_X){
					rtPoint.left++; rtPoint.right++;
					iDistance--;
				}
			}
			if(iDistance==0)
				iState=0;
			break;
		}
		if(rtPoint.bottom > MAP_Y)
			DeleteScourge();
	}
}