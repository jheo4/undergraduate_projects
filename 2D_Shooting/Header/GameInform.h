#pragma once
#include<Windows.h>
#include"Define.h"
#include"resource.h"

class CGameInform{
private:
	UINT iGameState;
	UINT iMenuState;
	TCHAR strScore[128];
	TCHAR strBattleHP[128];
	TCHAR strBattleMP[128];
	RECT rtScope;

	HBITMAP NewBitmap;
	HBITMAP OldBitmap;
public:
	CGameInform(void);
	~CGameInform(void);
	void SetGameState(UINT _iState){iGameState = _iState;};
	void SetMenuState(UINT _iState){iMenuState = _iState;};
	void SetStrScore(UINT _iScore){ wsprintf(strScore, TEXT("SCORE : %u"), _iScore); };
	void SetStrBattleHP(UINT _iHP){ wsprintf(strBattleHP, TEXT("My HP : %u"), _iHP);};
	void SetStrBattleMP(UINT _iMP){	wsprintf(strBattleMP, TEXT("My MP : %u"), _iMP);};
	RECT GetUpdateRect(){return rtScope;};
	UINT GetGameState(){return iGameState;};
	UINT GetMenuState(){return iMenuState;};
	void Draw(HDC &hdc, HDC &FrontDC, HDC &BackDC, HINSTANCE hInstance, HBITMAP &BackBitmap);
};

