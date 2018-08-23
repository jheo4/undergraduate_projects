#include "GameInform.h"


CGameInform::CGameInform(void){
	wsprintf(strScore, TEXT("SCORE : %u"), 0);
	wsprintf(strBattleHP, TEXT("My HP : %u"), 0);
	wsprintf(strBattleMP, TEXT("My MP : %u"), 0);
	rtScope.left = MAP_X +20;
	rtScope.right = rtScope.left+150;
	rtScope.top = 100;
	rtScope.bottom = 260;
	iGameState = LOADING;
	iMenuState = NORMAL;
}


CGameInform::~CGameInform(void)
{
}

void CGameInform::Draw(HDC &hdc, HDC &FrontDC, HDC &BackDC, HINSTANCE hInstance, HBITMAP &BackBitmap){
	switch(iGameState){
	case LOADING:
		OldBitmap = (HBITMAP)SelectObject(FrontDC, BackBitmap);
		NewBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_LOADING)); //로딩화면
		SelectObject(BackDC, NewBitmap);
		BitBlt(FrontDC, 0, 0, MAP_X+200, MAP_Y+50, BackDC, 0, 0, SRCCOPY);
		StretchBlt(hdc, 0, 0, MAP_X+200, MAP_Y, FrontDC, 0, 0, MAP_X+200, MAP_Y+50, SRCCOPY);
		SelectObject(FrontDC, OldBitmap);
		DeleteObject(NewBitmap);
		// Draw Loading
		break;
	case MENU:
		switch(iMenuState){
		case NORMAL:
			OldBitmap = (HBITMAP)SelectObject(FrontDC, BackBitmap);
			NewBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_MENU)); //로딩화면
			SelectObject(BackDC, NewBitmap);
			BitBlt(FrontDC, 0, 0, 640, 480, BackDC, 0, 0, SRCCOPY);
			StretchBlt(hdc, 0, 0, MAP_X+200, MAP_Y+50, FrontDC, 0, 0, 640, 480, SRCCOPY);
			SelectObject(FrontDC, OldBitmap);
			DeleteObject(NewBitmap);
			break;
		case DIRECTION:
			OldBitmap = (HBITMAP)SelectObject(FrontDC, BackBitmap);
			NewBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_DIRECTION)); //
			SelectObject(BackDC, NewBitmap);
			BitBlt(FrontDC, 0, 0, 700, 800, BackDC, 0, 0, SRCCOPY);
			StretchBlt(hdc, 0, 0, MAP_X+200, MAP_Y+50, FrontDC, 0, 0, 700, 800, SRCCOPY);
			SelectObject(FrontDC, OldBitmap);
			DeleteObject(NewBitmap);
			break;
		case NOTYET:
			OldBitmap = (HBITMAP)SelectObject(FrontDC, BackBitmap);
			NewBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_NOTYET)); //로딩화면
			SelectObject(BackDC, NewBitmap);
			BitBlt(FrontDC, 0, 0, 700, 750, BackDC, 0, 0, SRCCOPY);
			StretchBlt(hdc, 0, 0, MAP_X+200, MAP_Y+50, FrontDC, 0, 0, 700, 750, SRCCOPY);
			SelectObject(FrontDC, OldBitmap);
			DeleteObject(NewBitmap);
			break;
		case LOVEHHJ:
			OldBitmap = (HBITMAP)SelectObject(FrontDC, BackBitmap);
			NewBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_LOVEHHJ)); //로딩화면
			SelectObject(BackDC, NewBitmap);
			BitBlt(FrontDC, 0, 0, 700, 800, BackDC, 0, 0, SRCCOPY);
			StretchBlt(hdc, 0, 0, MAP_X+200, MAP_Y+50, FrontDC, 0, 0, 700, 800, SRCCOPY);
			SelectObject(FrontDC, OldBitmap);
			DeleteObject(NewBitmap);
			break;
		}
		break;
	case START:
	case RESTART:
		SetTextColor(hdc, RGB(0, 255, 0));
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, MAP_X + 20, 100, strScore, lstrlen(strScore));
		TextOut(hdc, MAP_X + 20, 200, strBattleHP, lstrlen(strBattleHP));
		TextOut(hdc, MAP_X + 20, 230, strBattleMP, lstrlen(strBattleMP));
		TextOut(hdc, MAP_X + 20, 700, TEXT("EXIT"), 4);
		break;
	case LOSE:
		OldBitmap = (HBITMAP)SelectObject(FrontDC, BackBitmap);
		NewBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_LOSE)); //로딩화면
		SelectObject(BackDC, NewBitmap);
		BitBlt(FrontDC, 0, 0, 700, 800, BackDC, 0, 0, SRCCOPY);
		StretchBlt(hdc, 0, 0, MAP_X+200, MAP_Y+50, FrontDC, 0, 0, 700, 800, SRCCOPY);
		
		SelectObject(FrontDC, OldBitmap);
		DeleteObject(NewBitmap);
		break;
	}

}