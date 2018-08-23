#pragma once
#include"unit.h"
#include"Define.h"
#include"resource.h"
class CScourge :public CUnit{
private :
	UINT iMove;
	UINT iDistance;
public:
	CScourge(void);
	~CScourge(void);
	void CreateScourge();
	void DeleteScourge(){bLife = FALSE; bDying = FALSE;};
	void Draw(HDC &hdc, HDC &FrontDC, HDC &BackDC, HINSTANCE hInstance, HBITMAP &BackBitmap);
	void Move();
};

