#pragma once
#include<Windows.h>
#include"Define.h"
#include"resource.h"
class CBullet{
protected:
	RECT rtPoint;
	BOOL bLife;
	UINT iAP;
	HBITMAP NewBitmap;
	HBITMAP OldBitmap;
	BOOL bDying;
public:
	CBullet(void);
	~CBullet(void);
	void DeleteBullet(){bLife = FALSE; bDying = FALSE;
	rtPoint.left = -1; rtPoint.right = -1; rtPoint.top = -1; rtPoint.bottom = -1;
	};	// Draw Exploded image;
	
	void Move(){};
	void Draw(HDC &hdc, HDC &FrontDC, HDC &BackDC, HINSTANCE hInstance, HBITMAP &BackBitmap){};
	void CeateBullet(RECT _rtBattlePoint, UINT _iAP){};
	void Collision(){bDying = TRUE; iAP=0;};
	BOOL IsDying(){return bDying;};
	UINT GetAP(){return iAP;};
	BOOL GetLife(){return bLife;};
	RECT GetPoint(){return rtPoint;};
};

