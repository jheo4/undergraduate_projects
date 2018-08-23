#pragma once
#include"Unit.h"
#include"Define.h"
#include"resource.h"
#include"MutalBullet.h"

enum{Motion1=1, Motion2, Motion3};
class CMutal : public CUnit{
public:
	CMutalBullet obMutalBullet;
	CMutal(void);
	~CMutal(void);
	void CreateMutal();
	void DeleteMutal();
	void Draw(HDC &hdc, HDC &FrontDC, HDC &BackDC, HINSTANCE hInstance, HBITMAP &BackBitmap);
	void Move();
	void SwitchDefaultMotion(){
		if(iState == Motion1) iState = Motion2;
		else if(iState == Motion2) iState = Motion3;
		else if(iState == Motion3) iState = Motion1;
	}
	void Attack(){
		if(bLife){
			if(!obMutalBullet.GetLife())
				obMutalBullet.CeateBullet(rtPoint, iAP);
		}
	}
};

