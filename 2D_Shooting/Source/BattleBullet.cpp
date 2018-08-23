#include "BattleBullet.h"

CBattleBullet::CBattleBullet(void) : CBullet(){
}


CBattleBullet::~CBattleBullet(void)
{
}


void CBattleBullet::CeateBullet(RECT _rtBattlePoint, UINT _iAP){
	rtPoint.top = _rtBattlePoint.top - 30;
	rtPoint.bottom = rtPoint.top + MYBULLET_H;
	rtPoint.left = _rtBattlePoint.left + 20;
	rtPoint.right = rtPoint.left+MYBULLET_W;
	bLife = TRUE; bDying = FALSE;
	iAP = _iAP;
}
void CBattleBullet::Draw(HDC &hdc, HDC &FrontDC, HDC &BackDC, HINSTANCE hInstance, HBITMAP &BackBitmap){
	//OldBitmap = (HBITMAP)SelectObject(FrontDC, BackBitmap);
	if(bDying) {
		NewBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BATTLEBULLETBOOM));		// Bullet Boom!! Img
		SelectObject(BackDC, NewBitmap);
		BitBlt(FrontDC, 0, 0, MYBULLETBOOM_W, MYBULLETBOOM_H, BackDC, 0, 0, SRCCOPY);
		TransparentBlt(hdc, rtPoint.left-2, rtPoint.top, MYBULLETBOOM_W, MYBULLETBOOM_H, FrontDC, 0, 0, MYBULLETBOOM_W, MYBULLETBOOM_H, RGB(255,255,255));
	}
	else{
		NewBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BATTLEBULLET)); 
		SelectObject(BackDC, NewBitmap);
		BitBlt(FrontDC, 0, 0, MYBULLET_W, MYBULLET_H, BackDC, 0, 0, SRCCOPY);
		TransparentBlt(hdc, rtPoint.left, rtPoint.top, MYBULLET_W, MYBULLET_H, FrontDC, 0, 0, MYBULLET_W, MYBULLET_H, RGB(255,255,255));
	}
	//SelectObject(FrontDC, OldBitmap);
	DeleteObject(NewBitmap);
	if(bDying)
		iAP=0;
}

void CBattleBullet::Move(){
	if(!bDying){
		rtPoint.top-=8;
		rtPoint.bottom-=8;
	
		if(rtPoint.top < 0)
			DeleteBullet();
	}
}