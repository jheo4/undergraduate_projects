#include<Windows.h>
#include"resource.h"
#include"TimerFunc.h"
#include"MyMap.h"
#include"Battle.h"
#include"BattleBullet.h"
#include"Mutal.h"
#include"GameInform.h"
#include"YamatoGun.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass=TEXT("StarCraft_201113122HeoJin");

/*	*	*	*	*	Jin's Global Value	*	*	*	*	*/
CMyMap obMap;
CBattle obBattle;
CBattleBullet *obBattleBullet;
CYamatoGun obYamato;
CMutal *obMutal; CScourge *obScourge;
CGameInform obGameInform;
UINT iMaxBullet = 5;
UINT iMaxMutal = 10; UINT iMaxScourge = 15;
BOOL bDelay=FALSE;
UINT iScore=0;
UINT iGameState=0;
RECT rtWorkSpace;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;

	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.hCursor=LoadCursor(g_hInst, MAKEINTRESOURCE(IDC_CURSOR1));
	WndClass.hIcon=LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW|CS_VREDRAW;
	RegisterClass(&WndClass);
	hWnd=CreateWindow(lpszClass, lpszClass, WS_CAPTION | WS_SYSMENU,
		0, 0, MAP_X+200, MAP_Y+50, 
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while(GetMessage(&Message, NULL, 0, 0)){
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){
	HDC hdc, FrontDC, BackDC;
	static HBITMAP BackMapBit;
	static HBITMAP BackBattleBit;
	static HBITMAP BackBattleBulletBit;
	static HBITMAP BackZergBit;
	static HBITMAP BackMutalBulletBit;
	static HBITMAP BackYAMATO;
	
	PAINTSTRUCT ps;
	switch(iMessage){
	case WM_CREATE:
		GetClientRect(hWnd, &rtWorkSpace);		// for Mouse
		SetTimer(hWnd, 1, 4000, LoadingNMenu);
		return 0;
	case WM_LBUTTONDOWN:
		UINT iPX, iPY;
		iPX = LOWORD(lParam);
		iPY = HIWORD(lParam);
		if(obGameInform.GetGameState()==MENU){
			switch(obGameInform.GetMenuState()){
			case NORMAL:
					switch(IsMenuSelect(iPX, iPY)){
					case SINGLE:
						obGameInform.SetGameState(START);
						SetTimer(hWnd, 1, 50, ScrollMapNInformUpdate);			// The 1 timer to scroll a map
						SetTimer(hWnd, 2, 100, DefaultMotion);		// The 2 timer for mutal and battle to switch default motion
						SetTimer(hWnd, 3, 100, AttackDelay);		// The 3 timer to delay inputing
						SetTimer(hWnd, 4, 20, MoveObject);		// The 4 timer to move obejcts
						SetTimer(hWnd, 5, 300, CreateNAttackMutal);		// The 5 timer for mutal to create and attack
						SetTimer(hWnd, 6, 500, CreateScourge);		// The 6 timer to create scourge
						SetTimer(hWnd, 7, 200, DeadEffect);		// The 7 timer to hold dead effects
						SetTimer(hWnd, 8, 3000, IsGameEnd);		// The 8 timer to judge EndGame					
						obBattle.CreateBattle();
						bDelay=FALSE; iScore=0;
						obBattleBullet = new CBattleBullet[iMaxBullet];
						obMutal = new CMutal[iMaxMutal]; obScourge = new CScourge[iMaxScourge];
						InvalidateRect(hWnd, NULL, TRUE);
						break;
						//Start Game
					case EDITOR:
						obGameInform.SetMenuState(DIRECTION);
						InvalidateRect(hWnd, NULL, TRUE);
						// HOW TO PLAY THE GAME
						break;
					case MULTI:
						obGameInform.SetMenuState(NOTYET);
						InvalidateRect(hWnd, NULL, TRUE);
						break;
						// Not Yet
					case EXIT:
						DestroyWindow(hWnd);
						return 0;
						// EXIT
					case INTRO:
						obGameInform.SetMenuState(LOVEHHJ);
						InvalidateRect(hWnd, NULL, TRUE);
						break;
						// Ester Egg
					}
				break;
			case DIRECTION:
				if(530 <= iPX && iPX <= 680 && 680 <= iPY && iPY <= 730){
					obGameInform.SetMenuState(NORMAL);
					InvalidateRect(hWnd, NULL, TRUE);
				}
				break;
			case NOTYET: 
			case LOVEHHJ:
				obGameInform.SetMenuState(NORMAL);
				InvalidateRect(hWnd, NULL, TRUE);
				// Ester Egg
				break;
			}
		}
		else if(obGameInform.GetGameState()==START){
			if(MAP_X+20 <iPX && iPX < MAP_X+40 &&
				700 < iPY && iPY < 720)
				DestroyWindow(hWnd);		// During start, Exit Button
		}
		else if(obGameInform.GetGameState()==LOSE){
			if(500 <= iPX && iPX <= 670 && 640 <= iPY && iPY <= 680){
					obGameInform.SetGameState(MENU);
					InvalidateRect(hWnd, NULL, TRUE);// During LoseActivity, Return to Menu Button
			}
			if(580 <= iPX && iPX <= 670 && 700 <= iPY && iPY <= 730){
					DestroyWindow(hWnd);		// During LoseActivity, Exit Button
				}/*
			*/
			///
		}
		return 0;
	case WM_KEYDOWN:
		switch(obGameInform.GetGameState()){
		case START:
			switch(wParam){
			case VK_UP:
			case VK_DOWN:
			case VK_RIGHT:
			case VK_LEFT:
			case VK_SPACE:
				if(bDelay==TRUE)
						break;
				obBattle.InputAction(wParam);
				if(obBattle.GetState()==Attack){
					for(UINT i=0; i<iMaxBullet; i++){
						if(!obBattleBullet[i].GetLife()){
							obBattleBullet[i].CeateBullet(obBattle.GetPoint(), obBattle.GetAP());
							bDelay = TRUE;
							break;
						}
					}
				}
				break;
			}
			break;
		}
		return 0;
	case WM_CHAR:
		switch(obGameInform.GetGameState()){
			case START:
			switch(wParam){
			case 'z':
			case 'Z':
				if(obBattle.GetMP()>=150 && !obYamato.bIs){
					obYamato.CeateBullet(obBattle.GetPoint(), 300);
					obBattle.SetState(Attack);
					obBattle.SetDirection(Stop);
					obBattle.ShootYamato();
				}
				break;
			}	
			break;
		}
		return 0;
	case WM_DESTROY:
		if(obGameInform.GetGameState()==START){
			delete []obBattleBullet; delete []obMutal; delete []obScourge;
		}
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		FrontDC=CreateCompatibleDC(hdc);
		BackDC=CreateCompatibleDC(hdc);
		
		switch(obGameInform.GetGameState()){
		case LOADING:
			BackMapBit = CreateCompatibleBitmap(hdc, MAP_X+200, MAP_Y+50);
			obGameInform.Draw(hdc, FrontDC, BackDC, g_hInst, BackMapBit);
			break;
		case LOSE:
		case MENU:
			BackMapBit = CreateCompatibleBitmap(hdc, MAP_X+200, MAP_Y+50);
			obGameInform.Draw(hdc, FrontDC, BackDC, g_hInst, BackMapBit);
			
			//Rectangle
			break;
		case START:
			BackMapBit = CreateCompatibleBitmap(hdc, MAP_X, MAP_Y);
			
			// 객체출력부분
			obMap.Draw(hdc, FrontDC, BackDC, g_hInst, BackMapBit);	// MAP DRAW
			if(obBattle.GetLife())
				obBattle.Draw(hdc, FrontDC, BackDC, g_hInst, BackMapBit);	// Battle Cruiser DRAW
			for(UINT i=0; i<iMaxMutal; i++){										// Mutalisks DRAW
				if(obMutal[i].GetLife()){
					obMutal[i].Draw(hdc, FrontDC, BackDC, g_hInst, BackMapBit);
					if(obMutal[i].obMutalBullet.GetLife())
						obMutal[i].obMutalBullet.Draw(hdc, FrontDC, BackDC, g_hInst, BackMapBit);
				}
			}
			for(UINT i=0; i<iMaxScourge; i++){									// Scourges DRAW
				if(obScourge[i].GetLife())
					obScourge[i].Draw(hdc, FrontDC, BackDC, g_hInst, BackMapBit);
			}
			for(UINT i=0; i<iMaxBullet; i++){									// BattleBullet Draw
				if(obBattleBullet[i].GetLife())
					obBattleBullet[i].Draw(hdc, FrontDC, BackDC, g_hInst, BackMapBit);
			}
			if(obYamato.GetLife())												// Yamamto Draw
				obYamato.Draw(hdc, FrontDC, BackDC, g_hInst, BackMapBit);
			// 		
			obGameInform.Draw(hdc, FrontDC, BackDC, g_hInst, BackMapBit);
			//
			DeleteObject(BackMapBit);
			break;
		}

		DeleteDC(FrontDC);
		DeleteDC(BackDC);
		EndPaint(hWnd, &ps);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
