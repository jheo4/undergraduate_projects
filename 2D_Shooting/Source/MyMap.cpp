#include "MyMap.h"


CMyMap::CMyMap(void) : iY(0), iHeight(0){
	rtMap.left=0; rtMap.right=MAP_X;
	rtMap.top=0; rtMap.bottom=MAP_Y;
}
CMyMap::~CMyMap(void){
}

void CMyMap::Draw(HDC &hdc, HDC &FrontDC, HDC &BackDC, HINSTANCE hInstance, HBITMAP &BackBitmap){
	NewBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_FIELD));

	OldBitmap = (HBITMAP)SelectObject(FrontDC, BackBitmap);
	SelectObject(BackDC, NewBitmap);
	
	BitBlt(FrontDC, 0, iY, MAP_X, MAP_Y, BackDC, 0, 0, SRCCOPY);
	if(iY!=0){
		BitBlt(FrontDC, 0, 0, MAP_X, iY, BackDC, 0, MAP_Y-iY, SRCCOPY);
	}
	BitBlt(hdc, 0, 0, MAP_X, MAP_Y, FrontDC, 0, 0, SRCCOPY);
	SelectObject(FrontDC, OldBitmap);
	DeleteObject(NewBitmap);
}

void CMyMap::ScrollMap(){
	iY+=2;
	if(iY > MAP_Y ) iY=0;
}