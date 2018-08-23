#pragma once
#include<Windows.h>
class CUnit
{
protected:
	HBITMAP Motion;
	HBITMAP OldBitmap;
	RECT rtPoint;
	UINT iState;	
	UINT iHP;
	UINT iAP;
	BOOL bLife;
	BOOL bDying;
public:
	CUnit(void);
	~CUnit(void);
	void Draw(HDC &hdc, HDC &FrontDC, HDC &BackDC, HINSTANCE hInstance, HBITMAP &BackBitmap){};
	void Move(){};
	void Suffer(UINT _iAP){
		if(iHP<=_iAP){
			iHP=0;
			bDying = TRUE;
		}
		else
			iHP=iHP-_iAP;
	}
	BOOL IsDying(){return bDying;};
	UINT GetHP(){return iHP;};
	UINT GetAP(){return iAP;};
	UINT GetState(){return iState;};
	RECT GetPoint(){return rtPoint;};
	BOOL GetLife(){return bLife;};
};

