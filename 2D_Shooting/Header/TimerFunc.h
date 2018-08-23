#pragma once
#include<Windows.h>
#include"MyMap.h"
#include"Battle.h"
#include"BattleBullet.h"
#include"Mutal.h"
#include"Scourge.h"
#include"Define.h"
#include"GameInform.h"
#include"YamatoGun.h"
extern CMyMap obMap;
extern CBattle obBattle;
extern CYamatoGun obYamato;
extern CBattleBullet *obBattleBullet;
extern CMutal *obMutal; extern CScourge *obScourge;
extern CGameInform obGameInform;
extern BOOL bDelay;
extern UINT iMaxBullet;
extern UINT iMaxMutal; extern UINT iMaxScourge;
extern UINT iScore;

/*	*	*	*	*	Check Collision Function	*	*	*	*	*/
BOOL IsCollision(RECT rtTarget, RECT rtBullet){	// 
	if( (rtTarget.left<=rtBullet.left && rtBullet.left <= rtTarget.right) 
		&&(rtTarget.top<=rtBullet.top && rtBullet.top <= rtTarget.bottom))
		return TRUE;
	if((rtTarget.left<=rtBullet.left && rtBullet.left <= rtTarget.right) 
		&&(rtTarget.top<=rtBullet.bottom && rtBullet.bottom <= rtTarget.bottom))
		return TRUE;
	if( (rtTarget.left<=rtBullet.right && rtBullet.right <= rtTarget.right) 
		&&(rtTarget.top<=rtBullet.top && rtBullet.top <= rtTarget.bottom))
		return TRUE;
	if((rtTarget.left<=rtBullet.right && rtBullet.right <= rtTarget.right) 
		&&(rtTarget.top<=rtBullet.bottom && rtBullet.bottom <= rtTarget.bottom))
		return TRUE;

	return FALSE;
}

/*	*	*	*	*	Select Menu Function	*	*	*	*	*/
UINT IsMenuSelect(UINT _iPX, UINT _iPY){
	RECT rtMenu[5];
	rtMenu[SINGLE].left=50; rtMenu[SINGLE].right=380; rtMenu[SINGLE].top=50; rtMenu[SINGLE].bottom=230; 
	rtMenu[EDITOR].left=400; rtMenu[EDITOR].right=600; rtMenu[EDITOR].top=170; rtMenu[EDITOR].bottom=400; 
	rtMenu[MULTI].left=50; rtMenu[MULTI].right=380; rtMenu[MULTI].top=300; rtMenu[MULTI].bottom=600; 
	rtMenu[EXIT].left=430; rtMenu[EXIT].right=700; rtMenu[EXIT].top=500; rtMenu[EXIT].bottom=750; 
	rtMenu[INTRO].left=20; rtMenu[INTRO].right=250; rtMenu[INTRO].top=680; rtMenu[INTRO].bottom=750; 
	for(int i=SINGLE; i<=INTRO; i++){
		if(rtMenu[i].left<= _iPX && _iPX <= rtMenu[i].right &&
			rtMenu[i].top<= _iPY && _iPY <= rtMenu[i].bottom)
			return i;
	}
	return 5;
};




//////////////////////////////////////////////////Timer Functions.//////////////////////

/*	*	*	*	*	DeadEffect Hold	*	*	*	*	*/
VOID CALLBACK DeadEffect(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime){
	if(obGameInform.GetGameState()!=START){
		KillTimer(hWnd, idEvent);
	}

	for(UINT i=0; i<iMaxScourge; i++){
		if(obScourge[i].IsDying()) obScourge[i].DeleteScourge();
	}
	for(UINT i=0; i<iMaxMutal; i++){
		if(obMutal[i].IsDying()) obMutal[i].DeleteMutal();
		if(obMutal[i].obMutalBullet.IsDying()) obMutal[i].obMutalBullet.DeleteBullet();
	}
	for(UINT i=0; i<iMaxBullet; i++){
		if(obBattleBullet[i].IsDying()) obBattleBullet[i].DeleteBullet();
	}
	InvalidateRect(hWnd, &obMap.GetMapRect(), FALSE);
}

/*	*	*	*	*	Loading Screen	*	*	*	*	*/
VOID CALLBACK LoadingNMenu(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime){//시작타이머
	if(obGameInform.GetGameState()==LOADING)
		obGameInform.SetGameState(MENU);
	InvalidateRect(hWnd, NULL, TRUE);
	KillTimer(hWnd, idEvent);
}


/*	*	*	*	*	ScrollMap & Inform Update	*	*	*	*	*/
VOID CALLBACK ScrollMapNInformUpdate(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime){
	if(obGameInform.GetGameState()!=START){
		KillTimer(hWnd, idEvent);
	}
	obMap.ScrollMap();
	if(obBattle.GetLife())
		obBattle.IncreaseMP();
	obGameInform.SetStrScore(iScore);
	obGameInform.SetStrBattleHP(obBattle.GetHP());
	obGameInform.SetStrBattleMP(obBattle.GetMP());
	InvalidateRect(hWnd, &obGameInform.GetUpdateRect(), TRUE);
}

/*	*	*	*	*	Default Motion Switch	*	*	*	*	*/
VOID CALLBACK DefaultMotion(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime){
	if(obGameInform.GetGameState()!=START){
		KillTimer(hWnd, idEvent);
	}
	if(obBattle.GetLife())
		obBattle.SwitchDefaultMotion();
	for(UINT i=0; i<iMaxMutal; i++){
		if(obMutal[i].GetLife()){
			obMutal[i].SwitchDefaultMotion();
			if(obMutal[i].obMutalBullet.GetLife())
				obMutal[i].obMutalBullet.SwitchMotion();
		}
	}
	InvalidateRect(hWnd, &obMap.GetMapRect(), FALSE);
}


/*	*	*	*	*	Judge End	*	*	*	*	*/
VOID CALLBACK IsGameEnd(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime){
	if(obGameInform.GetGameState()==START){
		if(obBattle.GetLife()==FALSE){
			delete []obBattleBullet; delete []obMutal; delete []obScourge;
			//패배입니다~
			obGameInform.SetGameState(LOSE);	// 패배화면
			InvalidateRect(hWnd, NULL, TRUE);
			KillTimer(hWnd, idEvent);
		}
	}
}

/*	*	*	*	*	Attack Delay	*	*	*	*	*/
VOID CALLBACK AttackDelay(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime){
	if(obGameInform.GetGameState()!=START){
		KillTimer(hWnd, idEvent);
	}
	if(bDelay == TRUE)
		bDelay=FALSE;
}

/*	*	*	*	*	Move & Check Collision	*	*	*	*	*/
VOID CALLBACK MoveObject(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime){
	if(obGameInform.GetGameState()!=START){
		KillTimer(hWnd, idEvent);
	}

	if(!obBattle.IsDying())
		obBattle.Move();

	// Check collision between Yamamto and Zerg
	if(obYamato.GetLife()){
		RECT rtYamato = obYamato.GetPoint();
		for(UINT i=0; i<iMaxScourge; i++){
			if(obScourge[i].GetLife()==TRUE){
				RECT rtScourge = obScourge[i].GetPoint();
				if(IsCollision(rtScourge, rtYamato)){
					obScourge[i].Suffer(obYamato.GetAP());
					if(obScourge[i].IsDying())
							iScore+=100;
				}
			}
		}
		for(UINT i=0; i<iMaxMutal; i++){
			if(obMutal[i].GetLife()==TRUE){
				RECT rtMutal = obMutal[i].GetPoint();
				if(IsCollision(rtMutal, rtYamato)){
					obMutal[i].Suffer(obYamato.GetAP());
					if(obMutal[i].IsDying())
						iScore+=300;
				}
			}
		}
		obYamato.Move();
		for(UINT i=0; i<iMaxScourge; i++){
			if(obScourge[i].GetLife()==TRUE){
				RECT rtScourge = obScourge[i].GetPoint();
				if(IsCollision(rtScourge, rtYamato)){
					obScourge[i].Suffer(obYamato.GetAP());
					if(obScourge[i].IsDying())
							iScore+=100;
				}
			}
		}
		for(UINT i=0; i<iMaxMutal; i++){
			if(obMutal[i].GetLife()==TRUE){
				RECT rtMutal = obMutal[i].GetPoint();
				if(IsCollision(rtMutal, rtYamato)){
					obMutal[i].Suffer(obYamato.GetAP());
					if(obMutal[i].IsDying())
						iScore+=300;
				}
			}
		}
	}


	for(UINT i=0; i<iMaxBullet; i++){
		if(obBattleBullet[i].GetLife()){
			obBattleBullet[i].Move();
			// Check collision between Battle bullet and Scourge
			RECT rtBullet = obBattleBullet[i].GetPoint();
			for(UINT j=0; j<iMaxScourge; j++){
				if(obScourge[j].GetLife()==TRUE){
					RECT rtScourge = obScourge[j].GetPoint();
					if(IsCollision(rtScourge, rtBullet)){
						obScourge[j].Suffer(obBattleBullet[i].GetAP());
						if(obScourge[j].IsDying())
							iScore+=100;
 						obBattleBullet[i].Collision();
					}
				}
			}
			// Check collision between Battle bullet and Mutal
			rtBullet = obBattleBullet[i].GetPoint();
			for(UINT j=0; j<iMaxMutal; j++){
				if(obMutal[j].GetLife()==TRUE){
					RECT rtMutal = obMutal[j].GetPoint();
					if(IsCollision(rtMutal, rtBullet)){
						obMutal[j].Suffer(obBattleBullet[i].GetAP());
						if(obMutal[j].IsDying())
							iScore+=300;
						obBattleBullet[i].Collision();
					}
				}
			}
		}
	}	

	// Check collision between Zerg and Battle
	for(UINT i=0; i<iMaxMutal; i++){
		if(obMutal[i].GetLife()){
			obMutal[i].Move();
			if(obMutal[i].obMutalBullet.GetLife()){
				obMutal[i].obMutalBullet.Move();
				RECT rtMutalBullet= obMutal[i].obMutalBullet.GetPoint();
				RECT rtBattle = obBattle.GetPoint();
				if(IsCollision(rtBattle, rtMutalBullet)){
					obBattle.Suffer(obMutal[i].obMutalBullet.GetAP());
					obMutal[i].obMutalBullet.Collision();
				}
			}
		}
	}
	// Battle Cruiser and Scourge
	for(UINT i=0; i<iMaxScourge; i++){
		if(obScourge[i].GetLife()){
			obScourge[i].Move();
			RECT rtScourge = obScourge[i].GetPoint();
			RECT rtBattle = obBattle.GetPoint();
			if(IsCollision(rtBattle, rtScourge)){
				obBattle.Suffer(obScourge[i].GetAP());
				obScourge[i].Suffer(obScourge[i].GetHP());
			}
		}
	}
	InvalidateRect(hWnd, &obMap.GetMapRect(), FALSE);
}

/*	*	*	*	*	Create Mutal & Attack	*	*	*	*	*/
VOID CALLBACK CreateNAttackMutal(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime){
	if(obGameInform.GetGameState()!=START){
		KillTimer(hWnd, idEvent);
	}
	for(UINT i=0; i<iMaxMutal; i++){
		if(!obMutal[i].GetLife()){
			obMutal[i].CreateMutal();
			break;
		}else{
			if(!obMutal[i].obMutalBullet.GetLife())
				obMutal[i].Attack();
		}
	}
}

/*	*	*	*	*	Create Scourge	*	*	*	*	*/
VOID CALLBACK CreateScourge(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime){
	if(obGameInform.GetGameState()!=START){
		KillTimer(hWnd, idEvent);
	}
	for(UINT i=0; i<iMaxMutal; i++){
		if(!obScourge[i].GetLife()){
			obScourge[i].CreateScourge();
			break;
		}
	}
}