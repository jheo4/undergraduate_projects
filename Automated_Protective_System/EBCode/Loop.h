#pragma once
#include "Global.h"
#include "Jin.h"

void Loop(){
	Jin_Initializing();
	int iMainMutex =1;
	while(1){
		switch(caOPcode[0]){
		case 'O' :	// Bunker state open...  External only
			if(iMainMutex == 1){
				//ClearBoard();
				Jin_External();
				iMainMutex--;
			}
			break;
		case 'C' : 	// Bunker state open...  Internal only
			if(iMainMutex == 1){
				//ClearBoard();
				Jin_Internal();
				iMainMutex--;
			} 
			break;
		default : if(iMainMutex == 0) iMainMutex++;
		} 
	}
	Jin_Destroy();
}