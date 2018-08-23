#include "Battle.h"

//enum{Attack=0, GoUp, Up, UpRight, Right, RightDown, Down, DownLeft, Left, UpLeft};

CBattle::CBattle(void){
	CreateBattle();
}
CBattle::~CBattle(void){}

void CBattle::CreateBattle(){
	iState = Stop;
	iDirection = Stop;
	bDefaultMotion=FALSE;
	iHP = 500; iAP = 80; bLife = TRUE; bDying = FALSE;
	rtPoint.left = MAP_X/2;
	rtPoint.right = (MAP_X/2)+MYUNIT_W;
	rtPoint.top = MAP_Y-MYUNIT_H;
	rtPoint.bottom = MAP_Y;
	iMP = 0;
	iBoomMotion=0;
}
void CBattle::DeleteBattle(){
	iState = -1;
	iDirection = -1;
	bDefaultMotion=FALSE;
	iHP = 0; iAP = 0; bLife = FALSE; bDying = FALSE;
	rtPoint.left = 0;
	rtPoint.right = 0;
	rtPoint.top = 0;
	rtPoint.bottom = 0;
	iMP = 0;
}
void CBattle::InputAction(WPARAM wParam){
	switch(wParam){
	case VK_UP:
		if(iDirection==Down){
			iDirection = Stop;
			iState = Stop;
		}else{
			iDirection=GoUp;	
			iState = GoUp;
		} break;
	case VK_DOWN:
		if(iDirection==GoUp){
			iDirection = Stop;
			iState = Stop;
		}else{
			iDirection=Down;	
			iState = Down;
		} break;
	case VK_RIGHT:
		if(iDirection==Left){
			iDirection = Stop;
			iState = Stop;
		}else{
			iDirection=Right;	
			iState = Right;
		} break;
	case VK_LEFT:
		if(iDirection==Right){
			iDirection = Stop;
			iState = Stop;
		}else{
			iDirection=Left;	
			iState = Left;
		} break;
	case VK_SPACE:
		iDirection=Stop;
		iState = Attack;
		break;
	/*case VK_		 야마토건 Add!!
		iDirection=Stop;
		iState = Attack;
		break;
		*/
	}
}

void CBattle::Move(){
	if(bLife && !bDying){
		switch(iDirection){
		case GoUp:
			if(rtPoint.top > 0){
				rtPoint.top-=3;
				rtPoint.bottom-=3;
			} break;
		case Down:
			if(rtPoint.bottom < MAP_Y-5){
				rtPoint.top+=3;
				rtPoint.bottom+=3;
			} break;
		case Left:
			if(rtPoint.left > 0){
				rtPoint.right-=3;
				rtPoint.left-=3;
			} break;
		case Right:
			if(rtPoint.right < MAP_X){
				rtPoint.right+=3;
				rtPoint.left+=3;
			} break;
		}
	}
}

void CBattle::Draw(HDC &hdc, HDC &FrontDC, HDC &BackDC, HINSTANCE hInstance, HBITMAP &BackBitmap){
	
	//OldBitmap = (HBITMAP)SelectObject(FrontDC, BackBitmap);
	if(bDying){
		// 폭바 그래픽 처리!
		switch(iBoomMotion){
		case 0:
		case 1:
			Motion = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BATTLEDESTROY1)); 
			iBoomMotion++; break;
		case 2:
		case 3:
			Motion = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BATTLEDESTROY2));
			 iBoomMotion++; break;
		case 4:
		case 5:
			Motion = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BATTLEDESTROY3)); 
			iBoomMotion++; break;
		}
		
		SelectObject(BackDC, Motion);
		BitBlt(FrontDC, 0, 0, 50, 50, BackDC, 0, 0, SRCCOPY);
		TransparentBlt(hdc, rtPoint.left-5, rtPoint.top-5, 50, 50, FrontDC, 0, 0, 50, 50, RGB(255,255,255));
		if(iBoomMotion==5)
			DeleteBattle();
	}else{
	switch(iState){
		case Attack :
			Motion = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_ATTACK)); break;
		case GoUp :
			Motion = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_GOUP)); break;
		case Stop :
			if(bDefaultMotion)
				Motion = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_GOUP));
			else
				Motion = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_UP)); 
			break;
		case Down :
			Motion = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_DOWN)); break;
		case Left :
			Motion = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_LEFT)); break;
		case Right :
			Motion = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_RIGHT)); break;
		case UpRight:
			Motion = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_UPRIGHT)); break;
		case UpLeft :
			Motion = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_UPLEFT)); break;
		case DownRight:
			Motion = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_DOWNRIGHT)); break;
		case DownLeft:
			Motion = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_DOWNLEFT)); break;
		}
		SelectObject(BackDC, Motion);
	
		if(iState == Attack){	// Attack 이면 40*45
			BitBlt(FrontDC, 0, 0, MYUNIT_W, MYUNIT_H+5, BackDC, 0, 0, SRCCOPY);
			TransparentBlt(hdc, rtPoint.left, rtPoint.top, MYUNIT_W, MYUNIT_H+5, FrontDC, 0, 0, MYUNIT_W, MYUNIT_H+5, RGB(255,255,255));
		}else{
			BitBlt(FrontDC, 0, 0, MYUNIT_W, MYUNIT_H, BackDC, 0, 0, SRCCOPY);
			TransparentBlt(hdc, rtPoint.left, rtPoint.top, MYUNIT_W, MYUNIT_H, FrontDC, 0, 0, MYUNIT_W, MYUNIT_H, RGB(255,255,255));
		}
	}
	//SelectObject(FrontDC, OldBitmap);
	DeleteObject(Motion);
}
