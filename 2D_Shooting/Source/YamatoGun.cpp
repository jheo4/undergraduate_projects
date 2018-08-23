#include "YamatoGun.h"


CYamatoGun::CYamatoGun(void)
{
}


CYamatoGun::~CYamatoGun(void)
{
}

void CYamatoGun::CeateBullet(RECT _rtBattlePoint, UINT _iAP){
	rtPoint.top = _rtBattlePoint.top - YAMATO_H;
	rtPoint.bottom = rtPoint.top + YAMATO_H;
	rtPoint.left = _rtBattlePoint.left + 2;
	rtPoint.right = rtPoint.left+YAMATO_W;
	bLife = TRUE; bDying = FALSE;
	iAP = _iAP;
	bIs =TRUE;
}

void CYamatoGun::Draw(HDC &hdc, HDC &FrontDC, HDC &BackDC, HINSTANCE hInstance, HBITMAP &BackBitmap){
	//OldBitmap = (HBITMAP)SelectObject(FrontDC, BackBitmap);

	NewBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_YAMATO)); 
	SelectObject(BackDC, NewBitmap);
	BitBlt(FrontDC, 0, 0, YAMATO_W, YAMATO_H, BackDC, 0, 0, SRCCOPY);
	TransparentBlt(hdc, rtPoint.left, rtPoint.top, YAMATO_W, YAMATO_H, FrontDC, 0, 0, YAMATO_W, YAMATO_H, RGB(255,255,255));

	//SelectObject(FrontDC, OldBitmap);
	DeleteObject(NewBitmap);
}

void CYamatoGun::Move(){
	rtPoint.top-=8;
	rtPoint.bottom-=8;
	if(rtPoint.top < 0){
		DeleteBullet();
		bIs = FALSE;
	}
}