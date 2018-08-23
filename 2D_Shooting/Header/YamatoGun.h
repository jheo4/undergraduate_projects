#pragma once
#include<Windows.h>
#include"bullet.h"
#include"resource.h"
class CYamatoGun :
	public CBullet
{
public:
	BOOL bIs;
	CYamatoGun(void);
	~CYamatoGun(void);
	void CeateBullet(RECT _rtBattlePoint, UINT _iAP);
	void Move();
	void Draw(HDC &hdc, HDC &FrontDC, HDC &BackDC, HINSTANCE hInstance, HBITMAP &BackBitmap);
};

