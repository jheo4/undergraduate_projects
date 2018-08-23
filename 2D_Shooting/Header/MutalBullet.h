#pragma once
#include<Windows.h>
#include"resource.h"
#include"Bullet.h"
#include"Define.h"
class CMutalBullet : public CBullet{
private :
	BOOL bMotion;
	BOOL bDirection;
public:
	CMutalBullet(void);
	~CMutalBullet(void);
	void CeateBullet(RECT _rtBattlePoint, UINT _iAP);
	void Move();
	void Draw(HDC &hdc, HDC &FrontDC, HDC &BackDC, HINSTANCE hInstance, HBITMAP &BackBitmap);
	void SwitchMotion(){
		if(bMotion)
			bMotion = FALSE;
		else
			bMotion = TRUE;
	}
};

