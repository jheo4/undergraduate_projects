#include "MutalBullet.h"


CMutalBullet::CMutalBullet(void) : CBullet(){
	bMotion = FALSE;
}
CMutalBullet::~CMutalBullet(void){}

void CMutalBullet::CeateBullet(RECT _rtMutalPoint, UINT _iAP){
	rtPoint.top = _rtMutalPoint.top + MUTAL_H;
	rtPoint.bottom = rtPoint.top + MUTALBULLET_H;
	rtPoint.left = _rtMutalPoint.left + MUTALBULLET_W;
	rtPoint.right = rtPoint.left+MUTALBULLET_W;
	bLife = TRUE; bDying = FALSE;
	iAP = _iAP;
	bDirection = rand()%2;
}

void CMutalBullet::Draw(HDC &hdc, HDC &FrontDC, HDC &BackDC, HINSTANCE hInstance, HBITMAP &BackBitmap){
	OldBitmap = (HBITMAP)SelectObject(FrontDC, BackBitmap);
	if(bDying) 
		NewBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_MUTALBULLETBOOM));		// Bullet Boom!! Img
	else{
		if(bMotion)
			NewBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_MUTALBULLET1)); 
		else
			NewBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_MUTALBULLET2)); 
	}
	SelectObject(BackDC, NewBitmap);
	BitBlt(FrontDC, 0, 0, MUTALBULLET_W, MUTALBULLET_H, BackDC, 0, 0, SRCCOPY);
	TransparentBlt(hdc, rtPoint.left, rtPoint.top, MUTALBULLET_W, MUTALBULLET_H, FrontDC, 0, 0, MUTALBULLET_W, MUTALBULLET_H, RGB(255,255,255));
	SelectObject(FrontDC, OldBitmap);
	DeleteObject(NewBitmap);
	if(bDying)
		iAP=0;
}

void CMutalBullet::Move(){
	if(!bDying){
		rtPoint.top+=6;
		rtPoint.bottom+=6;
		if(bDirection)
			if(rtPoint.right < MAP_X){
			rtPoint.left++;
			rtPoint.right++;
			}
		else
			if(rtPoint.left > 0){
			rtPoint.left--;
			rtPoint.right--;
			}

		if(rtPoint.top >= MAP_Y-10)
			DeleteBullet();
	}
}