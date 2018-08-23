#pragma once
#include<Windows.h>
#include"resource.h"
#include"Define.h"
#include"Bullet.h"
class CBattleBullet : public CBullet{
public:
	CBattleBullet(void);
	~CBattleBullet(void);
	void CeateBullet(RECT _rtBattlePoint, UINT _iAP);
	void Move();
	void Draw(HDC &hdc, HDC &FrontDC, HDC &BackDC, HINSTANCE hInstance, HBITMAP &BackBitmap);
};

