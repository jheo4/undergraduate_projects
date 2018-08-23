#pragma once
#include<Windows.h>
#include"resource.h"
#include"Define.h"
#include"Unit.h"
enum{Attack=0, GoUp, Stop, UpRight, Right, DownRight, Down, DownLeft, Left, UpLeft};

class CBattle : public CUnit{
private:
	BOOL bDefaultMotion;
	UINT iDirection;
	UINT iMP;
	UINT iBoomMotion;
public:
	CBattle(void);
	~CBattle(void);
	void Draw(HDC &hdc, HDC &FrontDC, HDC &BackDC, HINSTANCE hInstance, HBITMAP &BackBitmap);
	void Move();
	void InputAction(WPARAM wParam);
	void SwitchDefaultMotion(){
		if(iState == Attack)
			iState = Stop;
		if(bDefaultMotion)
			bDefaultMotion = FALSE;	// Switch default motion 
		else
			bDefaultMotion = TRUE;
	}
	void SetState(UINT _iState){iState = _iState;};
	void IncreaseMP(){
		if(iMP<250){
			iMP++;
		}
	}
	UINT GetMP(){return iMP;};
	void SetDirection(UINT _iDirection){
		iDirection = _iDirection;
	}
	void ShootYamato(){iMP-=150;};
	void CreateBattle();
	void DeleteBattle();
};

