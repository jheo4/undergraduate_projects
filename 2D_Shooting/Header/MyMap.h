#pragma once
#include<Windows.h>
#include"resource.h"
#include"Define.h"

class CMyMap
{
private:
	UINT iY;
	UINT iHeight;
	HBITMAP NewBitmap;
	HBITMAP OldBitmap;
	RECT rtMap;
public:
	CMyMap(void);
	~CMyMap(void);
	void Draw(HDC &hdc, HDC &FrontDC, HDC &BackDC, HINSTANCE hInstance, HBITMAP &BackBitmap);
	void ScrollMap();
	RECT GetMapRect(){return rtMap;};
};